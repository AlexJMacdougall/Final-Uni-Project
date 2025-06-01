
#include <iostream>

#include "Components/PlayerController.hpp"

PlayerController::PlayerController(Entity player, Camera2D* cameraPtr, Registry* registryPtr) :
	Script(player,registryPtr)
{
	m_Player = player;
	m_cameraPtr = cameraPtr;
	m_RegistryPtr = registryPtr;

	m_speed = 100;
	m_health = 100;
	m_slowdown = 1.0f;
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
		m_slowdown = 0.4;
	}
	else if (IsKeyReleased(KEY_Q))
	{
		m_slowdown = 1.0;
	}
	
	//Move entity then check for collision
	
	transform->position.x += directionX * m_speed * dt * m_slowdown;
	if (!(CheckCollision().empty())) { transform->position.x -= directionX * m_speed * dt; }
	transform->position.y += directionY * m_speed * dt * m_slowdown;
	if (!(CheckCollision().empty())) { transform->position.y -= directionY * m_speed * dt; }
	
	//Update camera pos
	m_cameraPtr->target = Vector2{ transform->position.x , transform->position.y };
}

void PlayerController::Damage(float damage)
{
	m_health -= damage;
	std::cout << "Took " << damage << " damage, health is now " << m_health<<std::endl;
}

bool PlayerController::CheckIfDead()
{
	std::cout << "In playerController for player " << m_AttachedEntity << " Health is " << m_health<<std::endl;
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
	return collidingEntities;
}
