#pragma once

#include<cstdint>
#include <map>

#include "Components/ScriptComponent.hpp"
#include "Registry.hpp"

using Entity = std::uint32_t;

class MeleeEnemyScript : public Script
{
public:
	MeleeEnemyScript(Entity entity,Registry* registryPtr,Entity player);

	void update(float dt) override;

	void ApplyDamage(float damage);
private:
	Entity m_PlayerEntity;

	//Enemy Attributes
	float m_Health;
	float m_Damage;
	float m_Speed;
	float m_MeleeRange;
	float m_AttackWindupTime;
	float m_AttackRecoveryTime;

	//Rules
	bool InMeleeRange = false;
	bool IsDead = false;
	bool DoingAttackWindup = false;
	bool DoingAttackRecovery = false;
	bool DoingAttackAnimation = false;
	bool CurrentlyAttacking = false;

	enum State 
	{
		Chase,
		Attack,
		AttackWindup,
		AttackRecovery,
		Dead
	};

	enum State m_CurrentState;

	//Timers
	std::map<std::string, float> m_Timers =
	{
		{"Stun",0.0f},
		{"Windup",0.0f},
		{"Recovery",0.0f}, 
	};
};