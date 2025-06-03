
#include <iostream>

#include "Scripts/PlayerController.hpp"

PlayerController::PlayerController(Entity player, Camera2D* cameraPtr, Registry* registryPtr) :
	Script(player,registryPtr)
{
	m_Player = player;
	m_cameraPtr = cameraPtr;
	m_RegistryPtr = registryPtr;

	m_speed = 100;
	m_health = 10;
	m_slowdown = 1.0f;

	//Entity that holds the sprite for the spellcasting overlay
	m_SpellcastingOverlay = m_RegistryPtr->CreateEntity();

	Vec2 playerPos = m_RegistryPtr->GetComponent<Transform2D>(m_Player)->position;
	m_RegistryPtr->AddComponent<Transform2D>(m_SpellcastingOverlay, Transform2D{ playerPos ,Vec2{2,2} });
	m_RegistryPtr->AddComponent<Sprite>(m_SpellcastingOverlay, { Vec2{2,2},"Player",2,true });

	//Put entity into InternalEntities so that Script destructor can destroy it
	m_InternalEntities.insert(m_SpellcastingOverlay);
}

void PlayerController::update(float dt)
{
	auto transform = m_RegistryPtr->GetComponent<Transform2D>(m_Player);

	int directionX = 0, directionY = 0;

	//Check for keypresses
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
	{
		m_RegistryPtr->GetComponent<AnimatedSprite>(m_Player)->currentAnimation = "Walk";
		directionX = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
		directionY = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
	}
	else
	{
		m_RegistryPtr->GetComponent<AnimatedSprite>(m_Player)->currentAnimation = "Idle";
	}
	
	if (IsKeyPressed(KEY_Q))
	{
		if(m_slowdown == 1.0f)
		{
			m_slowdown = 0.5f;
		
			m_CurrentSpell = {};

			m_RegistryPtr->GetComponent<Sprite>(m_SpellcastingOverlay)->hide = false;
			for (Entity entity : m_KnownSpellWords) { m_RegistryPtr->GetComponent<ScriptComponent>(entity)->GetScript<SpellWordScript>()->SetActive(true); }
		}
		else
		{
			m_slowdown = 1.0f;
			m_RegistryPtr->GetComponent<Sprite>(m_SpellcastingOverlay)->hide = true;
			for (Entity entity : m_KnownSpellWords) { m_RegistryPtr->GetComponent<ScriptComponent>(entity)->GetScript<SpellWordScript>()->SetActive(false); }

			if (m_CurrentSpell.size() == 3) { CastSpell(); }
		}
	}

	//Move entity then check for collision
	transform->position.x += directionX * m_speed * dt * m_slowdown;
	if (!(CheckCollision().empty())) { transform->position.x -= directionX * m_speed * dt; }
	transform->position.y += directionY * m_speed * dt * m_slowdown;
	if (!(CheckCollision().empty())) { transform->position.y -= directionY * m_speed * dt; }
	//Update camera pos
	m_cameraPtr->target = Vector2{ transform->position.x , transform->position.y };

	//Update following entities
	m_RegistryPtr->GetComponent<Transform2D>(m_SpellcastingOverlay)->position = m_RegistryPtr->GetComponent<Transform2D>(m_Player)->position;

	//Update spell word entities
	int numOfWordsKnown = m_KnownSpellWords.size();

	//Update icon positions
	float angleSizeRad = (2 * PI) / numOfWordsKnown;
	float currentAngle = 0;

	for (Entity entity : m_KnownSpellWords)
	{
		auto entityTransform = m_RegistryPtr->GetComponent<Transform2D>(entity);

		entityTransform->position.x = (transform->position.x) + 32 * cos(currentAngle);
		entityTransform->position.y = (transform->position.y) + 32 * sin(currentAngle);
		currentAngle += angleSizeRad;
	}

	//Update timers
	if (hitTimer > 0) { hitTimer -= dt; }
}

void PlayerController::Damage()
{
	if(hitTimer <= 0)
	{
		m_health -= 1;
		hitTimer = 0.3f;
		std::cout << "Damaged" << std::endl;
	}
}

bool PlayerController::CheckIfDead()
{
	return (m_health <= 0);
}

float PlayerController::GetHealth()
{
	return m_health;
}

float PlayerController::GetSpeed()
{
	return m_speed;
}

void PlayerController::AddLearnedSpellWord(Entity newWord) 
{
	m_KnownSpellWords.insert(newWord);
}

void PlayerController::AddSpellWord(Entity newWord)
{
	if(m_CurrentSpell.size() < 3) //Max words per spell is 3
	{
		m_CurrentSpell.insert(newWord);
	}
}

void PlayerController::RemoveSpellWord(Entity word)
{
	m_CurrentSpell.erase(word);
}

void PlayerController::CastSpell()
{
	std::vector<Entity> spellWords;

	for (Entity word : m_CurrentSpell) { spellWords.push_back(word); }

	Entity spell = m_RegistryPtr->CreateEntity();
	m_InternalEntities.insert(spell);

	float spellStartingOffset = 40.0f;

	auto mousePos = GetScreenToWorld2D(GetMousePosition(), *m_cameraPtr);
	auto playerPos = m_RegistryPtr->GetComponent<Transform2D>(m_Player)->position;

	float angle_radians = atan2(mousePos.y - playerPos.y, mousePos.x - playerPos.x);

	float xDir = cos(angle_radians);
	float yDir = sin(angle_radians);

	float xStartPos = playerPos.x + xDir * spellStartingOffset;
	float yStartPos = playerPos.y + yDir * spellStartingOffset;

	m_RegistryPtr->AddComponent<Transform2D>(spell, { Vec2{xStartPos,yStartPos},Vec2{0.75,0.75} });
	m_RegistryPtr->AddComponent<Sprite>(spell, *m_RegistryPtr->GetComponent<Sprite>(spellWords[1]));
	m_RegistryPtr->GetComponent<Sprite>(spell)->hide = false;
	m_RegistryPtr->GetComponent<Sprite>(spell)->Layer = 1;
	m_RegistryPtr->AddComponent<ScriptComponent>(spell,ScriptComponent());
	SpellScript* spellScript = new SpellScript(spell, m_RegistryPtr, Vec2{ xDir, yDir });
	m_RegistryPtr->GetComponent<ScriptComponent>(spell)->attachScript<SpellScript>(*spellScript);

	std::cout << spellWords[0] << " " << spellWords[1] << " " << spellWords[2] << std::endl;
}

std::set<Entity> PlayerController::CheckCollision()
{
	//Get all entities with box collider
	auto colliderEntities = m_RegistryPtr->GetEntitiesWithComponent<BoxCollider>();
	std::set<Entity> collidingEntities = {};

	//Remove collider of player entitiy
	colliderEntities.erase(m_AttachedEntity);

	//Box to box collision
	for (Entity colliderEntity : colliderEntities)
	{
		auto pos1 = m_RegistryPtr->GetComponent<Transform2D>(m_AttachedEntity)->position;
		auto pos2 = m_RegistryPtr->GetComponent<Transform2D>(colliderEntity)->position;

		auto collider1 = m_RegistryPtr->GetComponent<BoxCollider>(m_AttachedEntity);
		auto collider2 = m_RegistryPtr->GetComponent<BoxCollider>(colliderEntity);

		if(collider1->layer == collider2->layer)
		{
			//AABB positions for comparison
			float right1 = pos1.x + (collider1->width / 2);
			float left1 = pos1.x - (collider1->width / 2);

			float right2 = pos2.x + (collider2->width / 2);
			float left2 = pos2.x - (collider2->width / 2);

			float upper1 = pos1.y - (collider1->height / 2);
			float lower1 = pos1.y + (collider1->height / 2);

			float upper2 = pos2.y - (collider2->height / 2);
			float lower2 = pos2.y + (collider2->height / 2);

			if ((right1 >= left2) &&
				(left1 <= right2) &&
				(upper1 <= lower2) &&
				(lower1 >= upper2))
			{
				collidingEntities.insert(colliderEntity);
			}
		}
	}
	return collidingEntities;
}
