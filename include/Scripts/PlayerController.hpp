#pragma once

#include "Components/ScriptComponent.hpp"
#include "Scripts/SpellWordScript.hpp"
#include "Scripts/SpellScript.hpp"
#include "Registry.hpp"

//using Entity = uint32_t;

class PlayerController : public Script
{
public:
	PlayerController(Entity player, Camera2D* cameraPtr, Registry* registryPtr);

	void update(float dt) override;

	void Damage();
	bool CheckIfDead();
	float GetHealth();
	float GetSpeed();

	void AddLearnedSpellWord(Entity newWord);
	void AddSpellWord(Entity newWord);
	void RemoveSpellWord(Entity word);
	//Function that the system manager can call on the player when entering a new room
	void EnterNewRoom();
	void RemoveInternalEntity(Entity entity);

	void CreateSpellEntitiy();

	float m_slowdown;
private:
	Entity m_Player;
	Camera2D* m_cameraPtr;
	Registry* m_RegistryPtr;

	std::set<Entity> m_CurrentSpell;

	std::set<Entity> m_ActiveSpells;

	std::set<Entity> CheckCollision();

	float m_speed;
	int m_health;
	float hitTimer = 0.0f;

	//Spellcasting system
	Entity m_SpellcastingOverlay;

	std::set<Entity> m_KnownSpellWords;
};

