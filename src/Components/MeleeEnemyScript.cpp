
#include "Components/MeleeEnemyScript.hpp"

#include <iostream>

MeleeEnemyScript::MeleeEnemyScript(Entity entity,Registry* registryPtr,Entity player) : 
	Script(entity,registryPtr),
	m_PlayerEntity(player)
{
	m_Health = 10.0f;
	m_Damage = 100.0f;
	m_Speed = 90.0f;
	m_MeleeRange = 50.0f;

	m_Navmesh = m_RegistryPtr->GetEntitiesWithComponent<Navmesh>();
	Get_Path(m_PlayerEntity);

	m_CurrentState = Chase;
}

void MeleeEnemyScript::update(float dt)
{
	//Get entity sprite
	auto sprite = m_RegistryPtr->GetComponent<AnimatedSprite>(m_AttachedEntity);

	//Update rules
	InMeleeRange = (GetDistance(m_AttachedEntity, m_PlayerEntity) < m_MeleeRange);
	DoingAttackWindup = (!(sprite->finishedAnimation) && sprite->currentAnimation == "Windup");
	DoingAttack = (!(sprite->finishedAnimation) && sprite->currentAnimation == "Attack");
	DoingAttackRecovery = (!(sprite->finishedAnimation) && sprite->currentAnimation == "Recovery");
	//

	//Check if enemy is dead - outside of switch because it is the same regardless of state
	if (m_Health < 0.0f) { m_CurrentState = Dead; }

	switch (m_CurrentState)
	{
	case Dead:
		std::cout << "Enemy " << m_AttachedEntity << " is dead" << std::endl;
		m_RegistryPtr->DestroyEntity(m_AttachedEntity);
		break;

	case Chase:

		if (InMeleeRange) { 
			m_CurrentState = AttackWindup; 
			sprite->currentAnimation = "Windup";
		}
		else
		{
			sprite->currentAnimation = "Move";

			if (!m_MovePath.empty()) 
			{
				moveTarget = m_MovePath.front(); 
				Move_Towards(m_RegistryPtr->GetComponent<Transform2D>(moveTarget)->position, dt);
				if (GetDistance(moveTarget, m_AttachedEntity) <= 1)
				{
					m_MovePath.pop_front(); 
				}
			}
			else
			{
				Get_Path(m_PlayerEntity);
			}

			//DEBUG//
			/*
			auto currentPos = m_RegistryPtr->GetComponent<Transform2D>(m_AttachedEntity)->position;
			
			for (Entity test : testEntities) { m_RegistryPtr->DestroyEntity(test); }
			testEntities = {};

			for(Entity entity:m_MovePath)
			{
				auto entityPos = m_RegistryPtr->GetComponent<Transform2D>(entity)->position;
				Entity newEntity = m_RegistryPtr->CreateEntity();
				m_RegistryPtr->AddComponent<Transform2D>(newEntity, { {entityPos.x,entityPos.y},{1,1} });
				m_RegistryPtr->AddComponent<Sprite>(newEntity, Sprite{ {0,0},"Enemy",1 });
				testEntities.insert(newEntity);
			}
			*/
		}
		break;

	case AttackWindup:
		if (!DoingAttackWindup)
		{
			m_CurrentState = Attack;
			sprite->currentAnimation = "Attack";
		}
		break;

	case AttackRecovery:
		if (!DoingAttackRecovery)
		{
			m_CurrentState = Chase;
			sprite->currentAnimation = "Move";
			hitPlayer = false;
		}
		break;

	case Attack:
		if (!DoingAttack)
		{
			m_CurrentState = AttackRecovery;
			sprite->currentAnimation = "Recovery";
		}
		else
		{
			if(GetDistance(m_PlayerEntity,m_AttachedEntity) < m_MeleeRange && !hitPlayer)
			{
				m_RegistryPtr->GetComponent<ScriptComponent>(m_PlayerEntity)->GetScript<PlayerController>()->Damage(m_Damage);
				hitPlayer = true;
			}
		}
		break;
	}
}
void MeleeEnemyScript::ApplyDamage(float damage)
{
	m_Health -= damage;
}

void MeleeEnemyScript::Move_Towards(Vec2 target,float dt)
{
	auto pos = m_RegistryPtr->GetComponent<Transform2D>(m_AttachedEntity);

	float angle_radians = atan2(target.y - pos->position.y, target.x - pos->position.x);

	float xVal = cos(angle_radians) * m_Speed * dt;
	float yVal = sin(angle_radians) * m_Speed * dt;

	pos->position.x += xVal;
	pos->position.y += yVal;
}

void MeleeEnemyScript::Get_Path(Entity target)
{
	Vec2 currentPos = m_RegistryPtr->GetComponent<Transform2D>(m_AttachedEntity)->position;

	m_Navmesh = m_RegistryPtr->GetEntitiesWithComponent<Navmesh>();//Remove when enemies are added properly; needed because enemy is created before level

	m_MovePath = {};

	//Keep track of how many interations; block if too many
	int iter = 0;

	//Get first square
	Vec2 navmeshPos = { int((currentPos.x + 1) / 32),int((currentPos.y + 1) / 32) }; //32 is size of sprite; change if it is different
	Entity firstNavmeshPoint;

	for (Entity entity : m_Navmesh)
	{
		auto square = m_RegistryPtr->GetComponent<Navmesh>(entity);
		if (CompareVec2(square->pos, navmeshPos))
		{
			firstNavmeshPoint = entity;
			m_MovePath.splice(m_MovePath.begin(), Search(target, firstNavmeshPoint, std::set<Entity>{}, iter));
			break;
		}
	}


	//DEBUG ////////////// REMOVE
	//for (Entity entity : path) { std::cout << "Entity " << entity << std::endl; }
	//m_CurrentState = Dead;
	//DEBUG ////////////// REMOVE
}

std::list<Entity> MeleeEnemyScript::Search(Entity targetEntity, Entity currentNavmeshPoint,std::set<Entity> searchedPositions,int iter)
{
	if (iter > max_Iterations) { return std::list<Entity>{}; }

	Entity nextEntity;
	Vec2 nextPosition;
	Vec2 currentPos = m_RegistryPtr->GetComponent<Navmesh>(currentNavmeshPoint)->pos;

	std::list<Entity> path;

	//Create set of all adjacent navmesh positions
	std::list<Entity> possibleDirections;

	for (Vec2 direction : directionVectors)
	{
		Vec2 nextPosition = Vec2Add(currentPos, direction);
		for (Entity entity : m_Navmesh)
		{
			if (CompareVec2(m_RegistryPtr->GetComponent<Navmesh>(entity)->pos, nextPosition))
			{
				possibleDirections.push_back(entity);
			}
		}
	}

	//Check that there are possible directions
	if(possibleDirections.size() != 0)
	{
		//Find which is closest to the target pos
		float lowestDistance = 1000;
		Entity closestEntity = possibleDirections.front();

		for(Entity entity : possibleDirections)
		{
			float newDistance = GetDistance(entity, m_PlayerEntity);
			//Check that the entity hasn't been checked and that it is also closer to the target
			if(newDistance < lowestDistance && (searchedPositions.find(entity) == searchedPositions.end()))
			{
				if ((GetDistance(closestEntity, m_PlayerEntity)) < m_MeleeRange - 1) { return path; }
				lowestDistance = newDistance;
				closestEntity = entity;
				searchedPositions.insert(closestEntity);
			}
		}
		//Repeat starting from closest entity
		path.push_back(closestEntity);
		iter += 1;
		path.splice(path.end(), Search(m_PlayerEntity, closestEntity,searchedPositions,iter));
	}
	else
	{
		return std::list<Entity> {};
	}
	return path;
}
