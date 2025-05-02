#include "cstdint"
#include "array"

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 1000;

template <typename T> 
class ComponentArray 
{
public:
	ComponentArray()
	{
		mSize = 0;
		mSparseArray = {};
		mComponentArray = {};
	}

	void AddComponent(Entity entity,T component)
	{
		size_t newIndex = mSize;

		//Append Component to dense array at highest location
		mComponentArray[newIndex] = component;
		//Add index of component to sparse array at the Entity ID's index
		mSparseArray[entity] = newIndex;

		mSize += 1;
	}

	void RemoveComponent(Entity entity)
	{
		//Move last item to deleted item's spot
		uint32_t indexOfRemovedComponent = mSparseArray[entity];
		uint32_t indexOfLastComponent = m_size - 1;
		mComponentArray[indexOfRemovedComponent] = mComponentArray[indexOfLastComponent];

		//Update sparse array to point to correct index
		auto iter = std::find(mSparseArray.begin(), mSparseArray[mSize], indexOfLastComponent);
		Entiy entityOfLastComponent = std::distance(mSparseArray.begin(), iter);
		mSparseArray[entityOfLastComponent] = indexOfRemovedComponent;

		//Remove unnecessary data
		mComponentArray.std::erase(indexOfLastComponent);
		mSparseArray.std::erase(entity);

		mSize -= 1;
	}

private:
	//Sparse Array
	std::array<uint32_t, MAX_ENTITIES> mSparseArray;
	//Dense Array
	std::array<T, MAX_ENTITIES> mComponentArray;
	uint32_t mSize;
};
