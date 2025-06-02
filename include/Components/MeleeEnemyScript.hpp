#pragma once

#include<cstdint>
#include <map>

#include "Components/ScriptComponent.hpp"
#include "Components/StructComponents.hpp"
#include "Components/PlayerController.hpp"
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
	bool DoingAttack = false;
	bool Recalc = false;
	bool StopIdle = false;
	bool hitPlayer = false;

	enum State 
	{
		Chase,
		Attack,
		AttackWindup,
		AttackRecovery,
		Dead,
		Idle
	};

	enum State m_CurrentState;

	//Timers
	std::map<std::string, float> m_Timers =
	{
		{"Stun",0.0f},
		{"Idle",0.0f},
	};

	std::array<Vec2,4> directionVectors =
	{
		Vec2{0,1},
		Vec2{0,-1},
		Vec2{1,0},
		Vec2{-1,0}
	};

	//Pathing to player
	void Move_Towards(Vec2 target,float dt);
	void Get_Path(Entity target);
	std::list<Entity> Search(Entity targetEntity, Entity currentNavmeshPoint,std::set<Entity>,int iter);

	std::set<Entity> m_Navmesh;
	std::list<Entity> m_MovePath;

	Entity moveTarget;
	int max_Iterations = 30;

	std::set<Entity> testEntities;
};


