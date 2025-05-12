#include "System.hpp"
#include "iostream"


System::System()
{
}

void System::Run(float dt)
{
}

void System::AddEntities(std::set<Entity> newEntities)
{
	m_Entities.merge(newEntities);
}

std::set<Entity> System::GetEntities()
{
	return m_Entities;
}

DrawSystem::DrawSystem(std::shared_ptr<Registry> registryPtr)
{
	m_RegistryPtr = registryPtr;
}

void DrawSystem::Run(float dt)
{
	BeginDrawing();

	ClearBackground(BLACK);

	for(Entity entity : GetEntities())
	{
		auto texture = m_RegistryPtr->GetComponent<Texture>(entity);
		auto transform = m_RegistryPtr->GetComponent<Transform>(entity);

		//Draw Texture
		DrawTexture(*texture, transform->translation.x, transform->translation.y, WHITE);
	}

	EndDrawing();
}

PlayerController::PlayerController(std::shared_ptr<Registry> registryPtr)
{
	m_RegistryPtr = registryPtr;
}

void PlayerController::Run(float dt)
{
	for (Entity entity : GetEntities())
	{
		auto transform = m_RegistryPtr->GetComponent<Transform>(entity);

		float directionX = 0, directionY = 0;

		//Check for keypresses
		if(IsKeyDown(KEY_D) || IsKeyDown(KEY_A))
		{
			directionX = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
		}
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
		{
			directionY =  IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
		}

		transform->translation.x += directionX;
		transform->translation.y += directionY;

		//std::cout << m_RegistryPtr->GetComponent<Transform>(0)->translation.x << " " << m_RegistryPtr->GetComponent<Transform>(0)->translation.y << std::endl;
	}
}
