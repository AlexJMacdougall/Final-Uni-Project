#include "Registry.hpp"
#include "iostream"

Registry::Registry()
{
	m_EntityCount = 0;
	for (Entity ID = 0; ID < MAX_ENTITIES; ID++)
	{
		m_AvailableEntities.push(ID);
	}

	mSignatures->fill(Signature{ 0 });
}

Registry::~Registry()
{
	//Delete pointers to unallocate memory
	delete m_TransformComponents;
	delete m_TextureComponents;
}

Entity Registry::Create()
{
	Entity ID = m_AvailableEntities.front();
	m_AvailableEntities.pop();	

	m_EntityCount += 1;
	return ID;
}

void Registry::Destroy(Entity ID)
{
	m_AvailableEntities.push(ID);

	for (int i = 0; i < 16; i++) 
	{
			
	}
	
	mSignatures->at(ID) = Signature{ 0 };

	m_EntityCount -= 1;
}

const char* Registry::GetComponentTypename(ComponentType type)
{
	switch (type) {
	case 0:
		const char* typeName = typeid(Transform).name();
		return typeName;
	case 1:
		const char* typeName = typeid(Texture).name();
		return typeName;
	default:
		std::cout << "No component with signature " << type << std::endl;
		const char* typeName = "Unknown";
		return typeName;
	}
}
