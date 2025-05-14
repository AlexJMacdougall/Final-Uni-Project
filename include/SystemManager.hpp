#include <memory>

#include "Registry.hpp"
#include "Components.hpp"

using Entity = std::uint32_t;

class SystemManager
{
public:
	SystemManager::SystemManager(Registry* registryPtr);

	void SystemManager::Update(float dt);
	void SystemManager::Draw();
	void SystemManager::PlayerInput(float dt);
	float SystemManager::GetDistance(Entity entity1, Entity entity2);

	template<typename t>
	std::set<Entity> CheckCollision(Entity entity);
private:
	Registry* m_RegistryPtr;

	//Player Movement Variables
	float speed = 100;
};

template<typename t>
inline std::set<Entity> SystemManager::CheckCollision(Entity checkEntity)
{
	//Set to return
	std::set<Entity> collidingEntities = {};

	//Entities with two different collision types
	std::set<Entity> sphereColliderEntities = m_RegistryPtr->GetEntitiesWithComponent<SphereCollider>();
	std::set<Entity> boxColliderEntities = m_RegistryPtr->GetEntitiesWithComponent<BoxCollider>();

	//Remove entity being checked from sets of entities to check
	sphereColliderEntities.erase(checkEntity);
	boxColliderEntities.erase(checkEntity);

	if (typeid(t) == typeid(SphereCollider))
	{
		//Sphere to sphere collision
		for (Entity colliderEntity : sphereColliderEntities)
		{
			auto collider1 = m_RegistryPtr->GetComponent<SphereCollider>(checkEntity);
			auto collider2 = m_RegistryPtr->GetComponent<SphereCollider>(colliderEntity);

			float distance = GetDistance(checkEntity, colliderEntity);

			if ((distance - collider1->radius - collider2->radius) <= 0)
			{
				std::cout << "Collision" << std::endl;
				collidingEntities.insert(colliderEntity);
			}
		}
	}
	return collidingEntities;
}
