#include "Scripts/SpellScript.hpp"

SpellScript::SpellScript(Entity entity, Registry* registryPtr, Vec2 dir):
	Script(entity,registryPtr),
	m_dir(dir)
{
	m_Speed = 300.0f;
}

void SpellScript::update(float dt)
{
	auto pos = m_RegistryPtr->GetComponent<Transform2D>(m_AttachedEntity)->position;

	m_RegistryPtr->GetComponent<Transform2D>(m_AttachedEntity)->position = Vec2Add(pos, Vec2MultiplyFloat(m_dir, dt * m_Speed));

	if(CheckCollision().size() != 0)
	{
		//m_RegistryPtr->DestroyEntity(m_AttachedEntity);
	}
}

std::set<Entity> SpellScript::CheckCollision()
{
	//Get all entities with box collider
	auto colliderEntities = m_RegistryPtr->GetEntitiesWithComponent<BoxCollider>();
	std::set<Entity> collidingEntities = {};

	//Remove collider of attached entitiy
	colliderEntities.erase(m_AttachedEntity);

	//Box to box collision
	for (Entity colliderEntity : colliderEntities)
	{
		auto pos1 = m_RegistryPtr->GetComponent<Transform2D>(m_AttachedEntity)->position;
		auto pos2 = m_RegistryPtr->GetComponent<Transform2D>(colliderEntity)->position;

		auto collider1 = m_RegistryPtr->GetComponent<BoxCollider>(m_AttachedEntity);
		auto collider2 = m_RegistryPtr->GetComponent<BoxCollider>(colliderEntity);

		if (collider1->layer == collider2->layer)
		{
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