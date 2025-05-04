#include "bitset"
#include "memory"
#include "queue"
#include "unordered_map"

#include "ComponentArray.hpp"

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 16;

using Signature = std::bitset<MAX_COMPONENTS>;

class Registry {
public:
	Registry();
	//Returns new Entity
	Entity CreateEntity();
	//Destroys an entity and its components
	void DestroyEntity(Entity ID);

	//Add and Remove components from entities
	template<typename T>
	void AddComponent(Entity entity,T component);

	template<typename T>
	void RemoveComponent(Entity entity);

	//Get a pointer to ComponentArray of typename T
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray();

	//Add new component type
	template<typename T>
	void RegisterComponent();
private:
	//Current number of entities and available entity IDs
	Entity m_EntityCount;
	std::queue<Entity> m_AvailableEntities;

	//Holds entity signatures that keep track of what components it has
	std::array<Signature, MAX_ENTITIES>* m_Signatures = new std::array<Signature, MAX_ENTITIES>;

	//Map from typeName to componentArray - holds all component data
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays;
	//Map from typeName to Component type
	std::unordered_map<const char*, ComponentType> m_typenameToComponentTypes;
	//Map from Component type to typeName
	std::unordered_map<ComponentType,const char*> m_componentTypesToTypename;

	//Counter variable used to assign component type variables
	ComponentType mNextComponentType;
};

template<typename T>
inline void Registry::AddComponent(Entity entity,T component)
{
	const char* typeName = typeid(T).name();

	GetComponentArray<T>()->AddComponent(entity,component);

	//Update entity signature
	m_Signatures->at(entity).set(m_typenameToComponentTypes[typeName], true);
}

template<typename T>
inline void Registry::RemoveComponent(Entity entity)
{
	const char* typeName = typeid(T).name();

	GetComponentArray<T>()->RemoveComponent(entity);

	//Update entity signature
	m_Signatures->at(Entity).set(m_ComponentTypes[typeName], false);
}

template<typename T>
inline std::shared_ptr<ComponentArray<T>> Registry::GetComponentArray()
{
	const char* typeName = typeid(T).name();
	//Returns new std::shared_ptr to ComponentArray of type T
	return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
}

template<typename T>
inline void Registry::RegisterComponent()
{
	//Get typename of component to use as pointer to its Signature ID and 
	const char* typeName = typeid(T).name();

	//Insert pointer to new componentArray of type T mapped to typeName into m_ComponentArrays
	m_ComponentArrays.insert({ typeName,std::make_shared<ComponentArray<T>>()});

	//Get typename from componentID
	m_componentTypesToTypename.insert({ mNextComponentType,typeName });
	//Get componentID from typename
	m_typenameToComponentTypes.insert({ typeName,mNextComponentType });

	//Increment mNextComponentType by 1 for next component
	mNextComponentType += 1;
}
