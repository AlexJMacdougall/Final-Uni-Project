#include "Registry.hpp"
#include "iostream"

Registry::Registry()
{
	m_EntityCount = 0;
	for (Entity ID = 0; ID < MAX_ENTITIES; ID++)
	{
		m_AvailableEntities.push(ID);
	}

	m_Signatures->fill(Signature{ 0 });
	mNextComponentType = 0;
}

Entity Registry::CreateEntity()
{
	Entity entity = m_AvailableEntities.front();
	m_AvailableEntities.pop();	

	//Add entity to active entity set
	m_ActiveEntities.insert(entity);

	m_EntityCount += 1;
	return entity;
}

void Registry::DestroyEntity(Entity entity)
{
	Signature entitySignature = m_Signatures->at(entity);

	//Check entity signature and remove any entities it has
	for (int compId = 0; compId < MAX_COMPONENTS; compId++) {
		if(entitySignature.test(compId))
		{
			const char* typeName = m_componentTypesToTypename[compId];
			m_ComponentArrays[typeName]->RemoveComponent(entity);
		}
	}

	//Remove entity from active entity set
	m_ActiveEntities.erase(entity);

	//Push entity to back of available entity queue and reset signature
	m_AvailableEntities.push(entity);
	m_Signatures->at(entity) = Signature{ 0 };

	m_EntityCount -= 1;
}

Signature Registry::GetSignature(Entity entity)
{
	return m_Signatures->at(entity);
}
