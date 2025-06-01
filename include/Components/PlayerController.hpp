#pragma once

#include "Components/ScriptComponent.hpp"
#include "Registry.hpp"

using Entity = uint32_t;

class PlayerController : public Script
{
public:
	PlayerController(Entity player, Camera2D* cameraPtr, Registry* registryPtr);

	void update(float dt) override;

	void Damage(float damage);
	bool CheckIfDead();
	float GetHealth();
	float GetSpeed();

	float m_slowdown;
private:
	Entity m_Player;
	Camera2D* m_cameraPtr;
	Registry* m_RegistryPtr;

	std::set<Entity> CheckCollision();

	float m_speed;
	float m_health;
};

