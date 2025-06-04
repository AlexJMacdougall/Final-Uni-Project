#pragma once

#include "Components/ScriptComponent.hpp"
#include "Scripts/MeleeEnemyScript.hpp"
#include "Registry.hpp"

class SpellScript : public Script 
{
public:
	SpellScript(Entity entity, Registry* registryPtr,Vec2 dir);

	void update(float dt) override;
private:
	Vec2 m_dir;
	float m_Speed;
	float m_Damage = 10.0f;

	bool testCollision = true;

	std::set<Entity> CheckCollision();
};