#include "cstdint"
#include "bitset"
#include "array"
#include "memory"
#include "queue"

#include "ComponentArray.hpp"

#include "raylib.h"

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 16;

using Signature = std::bitset<MAX_COMPONENTS>;

class Registry {
public:
	Registry();
	~Registry();
	//Returns new Entity
	Entity Create();
	//Destroys an entity and its components
	void Destroy(Entity ID);

	//Add and Remove components from entities
	template<typename T>
	void AddComponent(Entity entity);

private:
	//Current number of entities and available entity IDs
	Entity m_EntityCount;
	std::queue<Entity> m_AvailableEntities;

	//Holds entity signatures that keep track of what components it has
	std::array<Signature, MAX_ENTITIES>* mSignatures = new std::array<Signature, MAX_ENTITIES>;

	//Gets component type
	const char* GetComponentTypename(ComponentType type);

	//Holds component data
	ComponentArray<Transform>* m_TransformComponents = new ComponentArray<Transform>();
	ComponentArray<Texture>* m_TextureComponents = new ComponentArray<Texture>();
};

template<typename T>
inline void Registry::AddComponent(Entity entity)
{

}