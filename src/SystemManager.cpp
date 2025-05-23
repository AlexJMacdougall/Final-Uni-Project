

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

void SystemManager::SetPlayer(Entity target)
{
	this->m_CameraTarget = target;

	auto targetPos = m_RegistryPtr->GetComponent<Transform>(m_CameraTarget)->translation;

	camera.target = Vector2{ targetPos.x, targetPos.y};
}

void SystemManager::Update(float dt)
{
	//Check if the current room has been loaded
	if(!m_LevelManagerPtr->loadedRoom)
	{
		m_LevelManagerPtr->loadedRoom = true;
		m_LevelManagerPtr->LoadCurrentRoom();
	}

	//Run entity scripts
	this->RunScripts(dt);

	//Draw all entities
	this->Draw();
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
				auto transform = m_RegistryPtr->GetComponent<Transform>(entity);

				Vector2 position = { transform->translation.x,transform->translation.y };
				Texture* spriteSheet = m_SpriteSheets[sprite->SpriteSheetID];

				//Draw Texture
				DrawTextureRec(*spriteSheet, sprite->textureRect, position, WHITE);
			}
		}
	}
	EndMode2D();
	EndDrawing();
}
/*
void SystemManager::PlayerInput(float dt)
{
	auto transform = m_RegistryPtr->GetComponent<Transform>(m_CameraTarget);

	int directionX = 0, directionY = 0;

	//Check for keypresses
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A))
	{
		directionX = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
	}
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
	{
		directionY = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
	}
	if (IsKeyPressed(KEY_Q))
	{
		m_Slowdown = 0.5;
	}
	else if (IsKeyReleased(KEY_Q))
	{
		m_Slowdown = 1.0;
	}

	if (IsKeyPressed(KEY_UP))
	{
		m_LevelManagerPtr->Move(3);
	}
	if (IsKeyPressed(KEY_DOWN))
	{
		m_LevelManagerPtr->Move(0);
	}
	if (IsKeyPressed(KEY_LEFT))
	{
		m_LevelManagerPtr->Move(1);
	}
	if (IsKeyPressed(KEY_RIGHT))
	{
		m_LevelManagerPtr->Move(2);
	}
	//Move entity then check for collision

	transform->translation.x += directionX * m_Speed * m_Slowdown;
	if (!(CheckCollision<BoxCollider>(m_CameraTarget).empty())) { transform->translation.x -= directionX * m_Speed; }
	transform->translation.y += directionY * m_Speed * m_Slowdown;
	if (!(CheckCollision<BoxCollider>(m_CameraTarget).empty())) { transform->translation.y -= directionY * m_Speed; }

	//Update camera pos
	auto cameraPos = m_RegistryPtr->GetComponent<Transform>(m_CameraTarget);
	camera.target = Vector2{ transform->translation.x , transform->translation.y };
}*/

float SystemManager::GetDistance(Entity entity1, Entity entity2)
{
	//Get translations of both entities
	auto pos1 =  m_RegistryPtr->GetComponent<Transform>(entity1)->translation;
	auto pos2 =  m_RegistryPtr->GetComponent<Transform>(entity2)->translation;

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
