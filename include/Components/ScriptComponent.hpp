#pragma once

#include <raylib.h>

#include "Registry.hpp"
#include "Components/StructComponents.hpp"

//Script class that provides a template to be overwritten by other scripts
class Script
{
public:
	Script::Script(Entity entity,Registry* registryPtr) : 
	m_AttachedEntity(entity),
	m_RegistryPtr(registryPtr)
	{};
	virtual void update(float dt) = 0;
protected:
	Entity m_AttachedEntity;
	
	Registry* m_RegistryPtr;

	float GetDistance(Entity entity1, Entity entity2)
	{
		//Get translations of both entities
		auto pos1 = m_RegistryPtr->GetComponent<Transform2D>(entity1)->position;
		auto pos2 = m_RegistryPtr->GetComponent<Transform2D>(entity2)->position;

		//Calculate x and y distance
		float xDist = pos1.x - pos2.x;
		float yDist = pos1.y - pos2.y;

		//Use pythagoras to calculate Distance
		float dist = sqrt((xDist * xDist) + (yDist * yDist));
		return abs(dist);
	}
};

//Component that can be added to entities to allow scripts to be attached
class ScriptComponent
{
public:
	ScriptComponent() = default;

	void update(float dt) { if (m_Script) { m_Script->update(dt); } }; //Runs attached script's update function

	template<typename T,typename ... Args>
	void attachScript(Args&& ...args)
	{
		T* scriptPtr = new T(std::forward<Args>(args)...); //Create a pointer to a new script and pass in the variadic arguments
		m_Script = scriptPtr; //Set script to T
	}

	template<typename T>
	T* GetScript()
	{
		return reinterpret_cast<T*>(m_Script);
	}

private:
	Script* m_Script{ nullptr }; //Pointer to script object, set as nullptr until assigned
};
