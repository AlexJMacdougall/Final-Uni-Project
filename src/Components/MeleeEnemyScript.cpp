
#include "Components/MeleeEnemyScript.hpp"

#include <iostream>

MeleeEnemyScript::MeleeEnemyScript(Entity entity,Registry* registryPtr,Entity player) : 
	Script(entity,registryPtr),
	m_PlayerEntity(player)
{
	m_Health = 10.0f;
	m_Damage = 2.0f;
	m_Speed = 12.0f;
	m_MeleeRange = 40.0f;
	m_AttackWindupTime = 3.0f;
	m_AttackRecoveryTime = 3.0f;

	m_CurrentState = Chase;
}

void MeleeEnemyScript::update(float dt)
{
	//Update rules
	InMeleeRange = (GetDistance(m_AttachedEntity, m_PlayerEntity) < m_MeleeRange);
	DoingAttackWindup = (m_Timers["Windup"] != 0);
	DoingAttackRecovery = (m_Timers["Recovery"] != 0);

	//Check if enemy is dead - outside of switch because it is the same regardless of state
	if (m_Health < 0.0f) { m_CurrentState = Dead; }
	/*
	switch (m_CurrentState)
	{
	case Dead:
		std::cout << "Enemy " << m_AttachedEntity << " is dead" << std::endl;
		m_RegistryPtr->DestroyEntity(m_AttachedEntity);
		break;

	case Chase:
		std::cout << "Chasing!" << std::endl;
		if (InMeleeRange) { 
			std::cout << "Starting Attack!" << std::endl;
			m_CurrentState = AttackWindup; 
		}
		break;
	case AttackWindup:
		if(!DoingAttackWindup)
	case AttackRecovery:
	case Attack:
		std::cout << "Attack" << std::endl;
		//Do attack//
		break;
	}
	*/
}
void MeleeEnemyScript::ApplyDamage(float damage)
{
	m_Health -= damage;
}

