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

Registry::~Registry()
{

}

Entity Registry::CreateEntity()
{
	Entity ID = m_AvailableEntities.front();
	m_AvailableEntities.pop();	

	m_EntityCount += 1;
	return ID;
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


	//Push entity to back of available entity queue and reset signature
	m_AvailableEntities.push(entity);
	m_Signatures->at(entity) = Signature{ 0 };

	m_EntityCount -= 1;
}
