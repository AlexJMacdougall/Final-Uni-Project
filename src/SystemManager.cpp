

#include <math.h>
#include <iostream>

#include <raylib.h>

#include "SystemManager.hpp"

SystemManager::SystemManager(Registry* registryPtr, int screenWidth,int screenHeight)
{
	m_RegistryPtr = registryPtr;
	m_LevelManager = LevelManager();
	m_LevelManager.SetReigstryPtr(m_RegistryPtr);

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	camera.target = Vector2{ 0.0f, 0.0f };
	camera.offset = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	m_SpriteSheets = 
	{ 
		{"LevelSprites",LoadTexture("LevelSprites.png"),{5,2},SPRITE_SIZE},
		{"Player",LoadTexture("PlayerSprites.png"),{4,3},SPRITE_SIZE},
		{"Enemy",LoadTexture("EnemySprites.png"),{5,4},SPRITE_SIZE},
		{"SpellWordIcons",LoadTexture("SpellWordIcons.png"),{6,1},SPRITE_SIZE},
	};

	ResetGame();

	std::array<SpellWordData, 6> spellTypes = 
	{
		SpellWordData{"First",3},
		SpellWordData{"Second",3},
		SpellWordData{"Third",3},
		SpellWordData{"Fourth",3},
		SpellWordData{"Fifth",3},
		SpellWordData{"Sixth",3}
	};

	//Create spell words - limited and hardcoded for demo
	for (int x = 0; x < 6; x++) { CreateSpellWord({ (float)x,0.0f },spellTypes[x]); }
	//Give the player all words to start with - only for demo
	for (Entity word : m_SpellWords) { m_RegistryPtr->GetComponent<ScriptComponent>(m_PlayerEntity)->GetScript<PlayerController>()->AddLearnedSpellWord(word); }

	SetTargetFPS(60);
}

SystemManager::~SystemManager()
{
	for (auto spriteSheet : m_SpriteSheets) { UnloadTexture(spriteSheet.textures); }
}

void SystemManager::Update(float dt)
{
	//Check if the player is dead
	if (m_RegistryPtr->GetComponent<ScriptComponent>(m_PlayerEntity)->GetScript<PlayerController>()->CheckIfDead())
	{
		ResetGame();
	}
	else
	{
		//Animate Sprites
		this->Animate(dt * m_Slowdown);

		//Draw all entities
		this->Draw();

		//Run entity scripts
		this->RunScripts(dt * m_Slowdown);

		//Check if the door entities have been interacted with
		for (Entity door : m_LevelManager.GetDoorEntities())
		{
			DoorScript* doorScript = m_RegistryPtr->GetComponent<ScriptComponent>(door)->GetScript<DoorScript>();
			if (doorScript->playerHasInteracted())
			{
				std::string movedDirection = doorScript->GetDirection();

				m_LevelManager.Move(movedDirection);
				m_LevelManager.LoadCurrentRoom();

				//Set player's position to next to the opposite door in the new room
				for (Entity oppositeDoor : m_LevelManager.GetDoorEntities())
				{
					DoorScript* oppositeDoorScript = m_RegistryPtr->GetComponent<ScriptComponent>(oppositeDoor)->GetScript<DoorScript>();
					if (oppositeDoorScript->GetDirection() == directionOpposites[movedDirection])
					{
						Vec2 oppositeDoorPos = m_RegistryPtr->GetComponent<Transform2D>(oppositeDoor)->position;
						Vec2 offset = Vec2MultiplyInt(directionVectors[directionOpposites[movedDirection]], 33);
						m_RegistryPtr->GetComponent<Transform2D>(m_PlayerEntity)->position = Vec2Minus(oppositeDoorPos, offset);
						break;
					}
				}
				break;
			}
		}
	}
}

void SystemManager::RunScripts(float dt)
{
	//Get entities that have script components
	std::set<Entity> entities = m_RegistryPtr->GetEntitiesWithComponent<ScriptComponent>();

	//Iterate over them and call update function
	for(Entity entity:entities) 
	{
		m_RegistryPtr->GetComponent<ScriptComponent>(entity)->update(dt);
	}

	m_Slowdown = m_RegistryPtr->GetComponent<ScriptComponent>(m_PlayerEntity)->GetScript<PlayerController>()->m_slowdown;
}

void SystemManager::Draw()
{
	BeginDrawing();

	ClearBackground(BLACK);

	BeginMode2D(camera);

	//Get entities that are drawable
	std::set<Entity> entities = m_RegistryPtr->GetEntitiesWithComponent<Sprite>();

	Rectangle textureRect = { 0,0,SPRITE_SIZE,SPRITE_SIZE };
	Rectangle positionRect;
	Vector2 origin;

	for (int currentLayer = 0; currentLayer < NUM_OF_LAYERS; currentLayer++)
	{
		for (Entity entity : entities)
		{
			//Get entity sprite
			auto sprite = m_RegistryPtr->GetComponent<Sprite>(entity);
			//Skip drawing sprite if hidden 
			if (!sprite->hide)
			{ 
				//Check the sprites layer
				if (sprite->Layer == currentLayer)
				{
					//If the sprite is on the layer currently being drawn, fetch other data needed and draw it
					auto transform = m_RegistryPtr->GetComponent<Transform2D>(entity);

					//If scale is equal to or less than zero it will not draw.
					assert(transform->scale.x > 0 && transform->scale.y > 0);

					//Set position
					positionRect.x = transform->position.x;
					positionRect.y = transform->position.y;

					//Scale texture
					positionRect.width = SPRITE_SIZE * transform->scale.x;
					positionRect.height = SPRITE_SIZE * transform->scale.y;

					//Set origin
					origin = { positionRect.width / 2, positionRect.height / 2 };

					SpriteSheet* spriteSheet = GetSpriteSheet(sprite->SpriteSheetID);
					Texture textures = spriteSheet->textures;

					//Set textureRect
					textureRect.x = sprite->UV.x * spriteSheet->spriteSize;
					textureRect.y = sprite->UV.y * spriteSheet->spriteSize;

					//Draw Texture
					DrawTexturePro(textures, textureRect, positionRect,origin, 0.0f, WHITE);
				}
			}
		}
	}

	EndMode2D();
	EndDrawing();
}

void SystemManager::Animate(float dt)
{
	//Get entities that have animated sprites
	std::set<Entity> entities = m_RegistryPtr->GetEntitiesWithComponent<AnimatedSprite>();

	for(Entity entity : entities)
	{
		auto animatedSprite = m_RegistryPtr->GetComponent<AnimatedSprite>(entity);
		bool updateSprite = false;

		//Check if the entities animation has changed
		if(animatedSprite->currentAnimation == animatedSprite->lastAnimationFrame)
		{
			//If the same animation is still being played, check if next frame should be shown
			if(animatedSprite->currentFrameTime > 0)
			{
				//If time is not finished for current frame, decrease timer
				animatedSprite->currentFrameTime -= dt;
			}
			else
			{
				//Check if increasing frame will exceed number of frames in animation
				if(animatedSprite->animationData[animatedSprite->currentAnimation].currentFrame + 1 == animatedSprite->animationData[animatedSprite->currentAnimation].frames)
				{
					//If on last frame of animation, reset to first one
					animatedSprite->animationData[animatedSprite->currentAnimation].currentFrame = 0;
					//Update finishedAnimation bool - used to check if animations that should only play once are finished
					animatedSprite->finishedAnimation = true;
				}
				else
				{
					//Increase frame by one
					animatedSprite->animationData[animatedSprite->currentAnimation].currentFrame += 1;
				}
				//Reset timer
				animatedSprite->currentFrameTime = animatedSprite->frameTime;
				updateSprite = true;
			}
		}
		else
		{
			//Update last animated frame to currentAnimation
			animatedSprite->lastAnimationFrame = animatedSprite->currentAnimation;
			//Set currentAnimation frame to zero
			animatedSprite->animationData[animatedSprite->currentAnimation].currentFrame = 0;
			//Reset frame timer
			animatedSprite->currentFrameTime = animatedSprite->frameTime;
			//Set finishedAnimation bool to false
			animatedSprite->finishedAnimation = false;
			updateSprite = true;
		}

		//If the frame has changed, update entity sprite
		if(updateSprite)
		{
			//Get sprite and spritesheet data
			auto sprite = m_RegistryPtr->GetComponent<Sprite>(entity);
			Vec2 spriteSheetSize = GetSpriteSheet(sprite->SpriteSheetID)->size;

			//Calculate UV
			Vec2 newUV = animatedSprite->animationData[animatedSprite->currentAnimation].startRect;

			for(int i = 0; i< animatedSprite->animationData[animatedSprite->currentAnimation].currentFrame;i++)
			{
				if (newUV.x + 1 < spriteSheetSize.x)
				{
					newUV.x += 1; 
				}
				else 
				{ 
					newUV.x = 0; 
					newUV.y += 1; 
				}
			}
			//Set sprite UV to current frame UV
			sprite->UV = Vec2Add(Vec2{ 0,0 },newUV);
		}
	}
}

SpriteSheet* SystemManager::GetSpriteSheet(std::string ID)
{
	for(SpriteSheet spriteSheet : m_SpriteSheets)
	{
		if (spriteSheet.ID == ID)
		{ 
			return &spriteSheet; 
		}
	}
}

float SystemManager::GetDistance(Entity entity1, Entity entity2)
{
	//Get translations of both entities
	auto pos1 =  m_RegistryPtr->GetComponent<Transform2D>(entity1)->position;
	auto pos2 =  m_RegistryPtr->GetComponent<Transform2D>(entity2)->position;

	//Calculate x and y distance
	float xDist = pos1.x - pos2.x;
	float yDist = pos1.y - pos2.y;

	//Use pythagoras to calculate Distance
	float dist = sqrt((xDist*xDist) + (yDist*yDist));
	return dist;
}

float SystemManager::GetSlowdownValue()
{
	return m_Slowdown;
}

void SystemManager::SetSlowdownValue(float slowdown)
{
	m_Slowdown = slowdown;
}

Camera2D* SystemManager::GetCamera()
{
	return &camera;
}

void SystemManager::ResetGame()
{
	if (m_PlayerEntity != -1) 
	{ 
		m_RegistryPtr->DestroyEntity(m_PlayerEntity); 
	}

	m_PlayerEntity = m_RegistryPtr->CreateEntity();


	//for (Entity entity : m_RegistryPtr->GetEntitiesWithComponent<Transform2D>()) { m_RegistryPtr->DestroyEntity(entity); }

	m_RegistryPtr->AddComponent<Transform2D>(m_PlayerEntity, Transform2D{ Vec2{150,150},Vec2{1,1} });
	m_RegistryPtr->AddComponent<BoxCollider>(m_PlayerEntity, BoxCollider{ 32.0f,32.0f });

	Sprite playerSprite = Sprite{ {0,0},"Player",2 };
	m_RegistryPtr->AddComponent<Sprite>(m_PlayerEntity, playerSprite);
	AnimatedSprite playerAnimations = AnimatedSprite{ &playerSprite,0.2f };
	playerAnimations.animationData["Idle"] = Animation{ Vec2{0,0},5 };
	playerAnimations.animationData["Walk"] = Animation{ Vec2{1,1},5 };
	playerAnimations.currentAnimation = "Idle";
	m_RegistryPtr->AddComponent<AnimatedSprite>(m_PlayerEntity, playerAnimations);

	PlayerController *playerController = new PlayerController(m_PlayerEntity, GetCamera(), m_RegistryPtr);

	ScriptComponent playerScript = ScriptComponent();
	playerScript.attachScript<PlayerController>(*playerController);
	std::cout << "Created playerScript and Controller" << std::endl;
	m_RegistryPtr->AddComponent<ScriptComponent>(m_PlayerEntity, playerScript);

	m_LevelManager.SetPlayer(m_PlayerEntity);
	m_LevelManager.GenerateLevel(10);
	m_LevelManager.LoadCurrentRoom();
	std::cout << "Leaving scope for player" << std::endl;
}

void SystemManager::CreateSpellWord(Vec2 UV,SpellWordData data)
{
	Entity spellWord = m_RegistryPtr->CreateEntity();
	m_RegistryPtr->AddComponent<Transform2D>(spellWord, { {100.0f,100.0f},Vec2{0.5,0.5}});
	m_RegistryPtr->AddComponent<SphereCollider>(spellWord, { 8.0f,1 });
	m_RegistryPtr->AddComponent<Sprite>(spellWord, Sprite{ UV,"SpellWordIcons",3,true });

	SpellWordScript *spellWordScript = new SpellWordScript(spellWord,m_RegistryPtr,m_PlayerEntity,&camera,data);
	ScriptComponent spellScriptComp = ScriptComponent();
	spellScriptComp.attachScript<SpellWordScript>(*spellWordScript);

	m_RegistryPtr->AddComponent<ScriptComponent>(spellWord, spellScriptComp);

	m_SpellWords.insert(spellWord);
}
