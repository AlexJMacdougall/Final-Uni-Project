#pragma once

#include "Components/ScriptComponent.hpp"
#include "Components/SpellWordScript.hpp"
#include "Registry.hpp"

//using Entity = uint32_t;

class PlayerController : public Script
{
public:
	PlayerController(Entity player, Camera2D* cameraPtr, Registry* registryPtr);

	void update(float dt) override;

	void Damage(float damage);
	bool CheckIfDead();
	float GetHealth();
	float GetSpeed();

	void AddSpellWord(Entity newSpell);

	float m_slowdown;
private:
	Entity m_Player;
	Camera2D* m_cameraPtr;
	Registry* m_RegistryPtr;

	std::set<Entity> CheckCollision();

	float m_speed;
	float m_health;

	//Spellcasting system
	Entity m_SpellcastingOverlay;

	std::set<Entity> m_KnownSpellWords;

	//Array that holds positions to display spell word icons
	std::array<Vec2,8> displayPos = 
	{

	}
};

