#include "System.hpp"
#include "iostream"

/*
System::System(std::shared_ptr<Registry> registryPtr)
{
	m_RegistryPtr = registryPtr;
}

void System::Run(float dt)
{
}


void System::AddEntities(std::set<Entity> newEntities)
{
	m_Entities.merge(newEntities);
}


void DrawSystem::Run(float dt)
{
	BeginDrawing();

	ClearBackground(BLACK);

	//Get entities that are drawable
	std::set<Entity> entities = m_RegistryPtr->GetEntitiesWithComponent<Texture>();

	for(Entity entity : entities)
	{
		auto texture = m_RegistryPtr->GetComponent<Texture>(entity);
		auto transform = m_RegistryPtr->GetComponent<Transform>(entity);

		//Draw Texture
		DrawTexture(*texture, transform->translation.x, transform->translation.y, WHITE);
	}

	EndDrawing();
}

void PlayerController::Run(float dt)
{
	//Temporary; entity zero is the player
	auto transform = m_RegistryPtr->GetComponent<Transform>(0);

	float directionX = 0, directionY = 0;

	//square collision test

	//Check for keypresses
	if(IsKeyDown(KEY_D) || IsKeyDown(KEY_A))
	{
		//collision test is left/right?
		directionX = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
	}
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
	{
		directionY =  IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
	}
	transform->translation.x += directionX * dt * speed;
	transform->translation.y += directionY * dt * speed;
}

void PhysicsSystem::Run(float dt)
{
	//Get entities that have physics components
	std::set<Entity> entities = m_RegistryPtr->GetEntitiesWithComponent<Physics>();

	for(Entity entity:entities)
	{
		//Get components
		auto transform = m_RegistryPtr->GetComponent<Transform>(entity);
		auto physics = m_RegistryPtr->GetComponent<Physics>(entity);

		//Move entity based on it's velocity
		transform->translation.x += physics->velocity.x;
		transform->translation.y += physics->velocity.y;
	}
}

void CollisionSystem::Run(float dt)
{
	//Sphere to sphere collision
}
*/