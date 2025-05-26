
#include "Components/DoorScript.hpp"
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
	if (GetDistance(m_AttachedEntity, m_PlayerEntity) < m_InteractRange && IsKeyPressed(KEY_E)) 
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