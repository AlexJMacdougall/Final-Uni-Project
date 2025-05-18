

#include <math.h>
#include <iostream>

#include <raylib.h>

#include "SystemManager.hpp"

SystemManager::SystemManager(Registry* registryPtr,int screenWidth,int screenHeight)
{
	m_RegistryPtr = registryPtr;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	camera.target = Vector2{ 0.0f, 0.0f };
	camera.offset = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	SetTargetFPS(60);
}

void SystemManager::SetPlayer(Entity target)
{
	this->cameraTarget = target;

	auto targetPos = m_RegistryPtr->GetComponent<Transform>(cameraTarget)->translation;

	camera.target = Vector2{ targetPos.x, targetPos.y};
}

void SystemManager::Update(float dt)
{
	this->PlayerInput(dt);
	this->Draw();
}

void SystemManager::Draw()
{
	BeginDrawing();

	ClearBackground(BLACK);

	BeginMode2D(camera);

	//Get entities that are drawable
	std::set<Entity> entities = m_RegistryPtr->GetEntitiesWithComponent<Texture>();

	for (Entity entity : entities)
	{
		//Get entity texture, position and scale.
		//Copying rather than referencing texture to not repeatedly scale  
		auto texture = *m_RegistryPtr->GetComponent<Texture>(entity);
		auto transform = m_RegistryPtr->GetComponent<Transform>(entity);

		Vector3 position = transform->translation;
		Vector3 scale = transform->scale;
		
		//Check entity scale is not zero
		//If you failed this assertion you added a texture to an entity with a scale of zero in x or y; therefore it will not draw
		assert((scale.x != 0) && (scale.y != 0));

		//Scale texture
		texture.height *= (int)scale.x;
		texture.width *= (int)scale.y;

		//Draw Texture
		DrawTexture(texture, position.x - (texture.width / 2), position.y - (texture.height / 2), WHITE);
	}
	EndMode2D();
	EndDrawing();
}

void SystemManager::PlayerInput(float dt)
{
	//Need assertion to protect player
	auto transform = m_RegistryPtr->GetComponent<Transform>(cameraTarget);

	float directionX = 0, directionY = 0;

	//Check for keypresses
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A))
	{
		directionX = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
	}
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
	{
		directionY = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
	}

	//Move entity then check for collision
	transform->translation.x += directionX * dt * speed;
	if(!(CheckCollision<BoxCollider>(cameraTarget).empty())){ transform->translation.x -= directionX * dt * speed; }
	transform->translation.y += directionY * dt * speed;
	if (!(CheckCollision<BoxCollider>(cameraTarget).empty())) { transform->translation.y -= directionY * dt * speed; }

	//Update camera pos
	auto cameraPos = m_RegistryPtr->GetComponent<Transform>(cameraTarget);
	camera.target = Vector2{ transform->translation.x , transform->translation.y};
}

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