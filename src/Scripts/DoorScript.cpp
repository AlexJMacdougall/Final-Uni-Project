
#include "Scripts/DoorScript.hpp"
#include <iostream>

DoorScript::DoorScript(Entity entity, Registry* registryPtr, Entity player, std::string dir) :
	Script(entity,registryPtr),
	m_PlayerEntity(player),
	m_Direction(dir)
{
	m_InteractRange = 40;
}

void DoorScript::update(float dt)
{	
	auto pos1 = m_RegistryPtr->GetComponent<Transform2D>(m_AttachedEntity)->position;
	auto pos2 = m_RegistryPtr->GetComponent<Transform2D>(m_PlayerEntity)->position;
	if (GetDistance(pos1, pos2) < m_InteractRange && IsKeyPressed(KEY_E))
	{ 
		m_PlayerInteracted = true;
	}
}

bool DoorScript::playerHasInteracted()
{
	return m_PlayerInteracted;
}

std::string DoorScript::GetDirection()
{
	return m_Direction;
}