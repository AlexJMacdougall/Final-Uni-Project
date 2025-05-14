#pragma once

#include <math.h>
#include <iostream>

#include <raylib.h>

#include "SystemManager.hpp"

SystemManager::SystemManager(Registry* registryPtr)
{
	m_RegistryPtr = registryPtr;
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
		
		//Draw Texture, offset by half texture scale
		DrawTexture(texture, position.x - (texture.width / 2), position.y - (texture.height / 2), WHITE);
	}

	EndDrawing();
}

void SystemManager::PlayerInput(float dt)
{
	//Temporary; entity zero is the player
	auto transform = m_RegistryPtr->GetComponent<Transform>(0);

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
	transform->translation.x += directionX * dt * speed;
	transform->translation.y += directionY * dt * speed;
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
