#pragma once

#include "Components/ScriptComponent.hpp"
#include "Registry.hpp"

class SpellScript : public Script 
{
public:
	SpellScript(Entity entity, Registry* registryPtr,Vec2 dir);

	void update(float dt) override;
private:
	Vec2 m_dir;
	float m_Speed;

	std::set<Entity> CheckCollision();
};