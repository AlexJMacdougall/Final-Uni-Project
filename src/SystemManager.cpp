

#include <math.h>
#include <iostream>

#include <raylib.h>

#include "SystemManager.hpp"

SystemManager::SystemManager(Registry* registryPtr, LevelManager* levelPtr,int screenWidth,int screenHeight)
{
	m_RegistryPtr = registryPtr;
	m_LevelManagerPtr = levelPtr;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	camera.target = Vector2{ 0.0f, 0.0f };
	camera.offset = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	LevelSprites = LoadTexture("LevelSprites.png");
	PlayerSprites = LoadTexture("PlayerSprites.png");

	m_SpriteSheets = 
	{ 
		&LevelSprites,
		&PlayerSprites 
	};

	SetTargetFPS(60);
}

SystemManager::~SystemManager()
{
	for (auto texPtr : m_SpriteSheets) { UnloadTexture(*texPtr); }
}

void SystemManager::Update(float dt)
{
	//Draw all entities
	this->Draw();

	//Run entity scripts
	this->RunScripts(dt);

	//Check if the door entities have been interacted with
	for(Entity door:m_LevelManagerPtr->GetDoorEntities())
	{
		DoorScript* doorScript = m_RegistryPtr->GetComponent<ScriptComponent>(door)->GetScript<DoorScript>();
		if(doorScript->playerHasInteracted())
		{
			std::string movedDirection = doorScript->GetDirection();

			m_LevelManagerPtr->Move(movedDirection);
			m_LevelManagerPtr->LoadCurrentRoom();

			//Set player's position to next to the opposite door in the new room
			for (Entity oppositeDoor : m_LevelManagerPtr->GetDoorEntities())
			{
				DoorScript* oppositeDoorScript = m_RegistryPtr->GetComponent<ScriptComponent>(oppositeDoor)->GetScript<DoorScript>();
				if(oppositeDoorScript->GetDirection() == directionOpposites[movedDirection])
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

	/*
	if (m_LevelManagerPtr->doorInteracted != "None")
	{
		std::string movedDirection = m_LevelManagerPtr->doorInteracted;
		m_LevelManagerPtr->Move(movedDirection);
		m_LevelManagerPtr->LoadCurrentRoom();
		m_LevelManagerPtr->doorInteracted = "None";

		//Set player's position to next to the opposite door in the new room
		Entity door = m_LevelManagerPtr->GetDoorEntity(directionOpposites[movedDirection]);
		Vec2 doorPos = m_RegistryPtr->GetComponent<Transform2D>(door)->position;
		Vec2 offset = Vec2MultiplyInt(directionVectors[directionOpposites[movedDirection]],33);

		m_RegistryPtr->GetComponent<Transform2D>(m_PlayerEntity)->position = Vec2Minus(doorPos, offset);
	}
	*/

	//Check if the first room has been loaded
	if (!m_LevelManagerPtr->loadedFirstRoom)
	{
		m_LevelManagerPtr->loadedFirstRoom = true;
		m_LevelManagerPtr->LoadCurrentRoom();
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
}

void SystemManager::Draw()
{
	BeginDrawing();

	ClearBackground(BLACK);

	BeginMode2D(camera);

	//Get entities that are drawable
	std::set<Entity> entities = m_RegistryPtr->GetEntitiesWithComponent<Sprite>();

	for (int currentLayer = 0; currentLayer < NUM_OF_LAYERS; currentLayer++)
	{
		for (Entity entity : entities)
		{
			//Get entity sprite
			auto sprite = m_RegistryPtr->GetComponent<Sprite>(entity);
			//Check the sprites layer
			if(sprite->Layer == currentLayer)
			{
				//If the sprite is on the layer currently being drawn, fetch other data needed and draw it
				auto transform = m_RegistryPtr->GetComponent<Transform2D>(entity);

				Vector2 position = { transform->position.x,transform->position.y };
				Texture* spriteSheet = m_SpriteSheets[sprite->SpriteSheetID];

				//Draw Texture
				DrawTextureRec(*spriteSheet, sprite->textureRect, position, WHITE);
			}
		}
	}
	EndMode2D();
	EndDrawing();
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

void SystemManager::SetPlayer(Entity target)
{
	this->m_PlayerEntity = target;

	auto targetPos = m_RegistryPtr->GetComponent<Transform2D>(m_PlayerEntity)->position;

	camera.target = Vector2{ targetPos.x, targetPos.y };
}