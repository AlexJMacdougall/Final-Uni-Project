#include "System.hpp"
#include "iostream"

void System::Run()
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

void DrawSystem::Run()
{
	BeginDrawing();

	ClearBackground(BLACK);

	for(Entity entity : GetEntities())
	{
		auto& texture = m_RegistryPtr->GetComponent<Texture>(entity);
		auto& transform = m_RegistryPtr->GetComponent<Transform>(entity);

		//Draw Texture
		DrawTexture(texture, transform.translation.x, transform.translation.y, WHITE);
	}

	EndDrawing();
}
