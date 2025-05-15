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
				collidingEntities.insert(colliderEntity);
			}
		}
	}
	else if (typeid(t) == typeid(BoxCollider))
	{
		//Box to box collision
		for (Entity colliderEntity : boxColliderEntities)
		{
			auto pos1 = m_RegistryPtr->GetComponent<Transform>(checkEntity)->translation;
			auto pos2 = m_RegistryPtr->GetComponent<Transform>(colliderEntity)->translation;

			auto collider1 = m_RegistryPtr->GetComponent<BoxCollider>(checkEntity);
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
	}
	return collidingEntities;
}
