
#include "Components/DoorScript.hpp"
#include <iostream>

DoorScript::DoorScript(Entity entity, Entity player, Registry* registryPtr,std::string* interactDirection, std::string dir) :
	Script(),
	m_Entity(entity),
	m_PlayerEntity(player),
	m_RegistryPtr(registryPtr),
	m_InteractDirection(interactDirection),
	m_Direction(dir)
{
	m_InteractRange = 40;
}

void DoorScript::update(float dt)
{	
	if (GetDistance(m_Entity, m_PlayerEntity) < m_InteractRange && IsKeyPressed(KEY_E)) 
	{ 
		*m_InteractDirection = m_Direction;
	}
}

float DoorScript::GetDistance(Entity entity1, Entity entity2)
{
	//Get translations of both entities
	auto pos1 = m_RegistryPtr->GetComponent<Transform2D>(entity1)->position;
	auto pos2 = m_RegistryPtr->GetComponent<Transform2D>(entity2)->position;

	//Calculate x and y distance
	float xDist = pos1.x - pos2.x;
	float yDist = pos1.y - pos2.y;

	//Use pythagoras to calculate Distance
	float dist = sqrt((xDist * xDist) + (yDist * yDist));
	return dist;
}