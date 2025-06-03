#pragma once

#include "Components/ScriptComponent.hpp"
#include "Scripts/PlayerController.hpp"
#include "Registry.hpp"

using Entity = uint32_t;

class SpellWordScript : public Script
{
public:
	SpellWordScript(Entity attachedEntity, Registry* registryPtr,Entity Player,Camera2D* camera,SpellWordData data);
	void update(float dt) override;
	void SetActive(bool val);
	SpellWordData* GetData();
private:
	bool m_Active = false;
	Entity m_PlayerEntity;
	Camera2D* m_CameraPtr;
	SpellWordData m_WordData;

	//Spell data
};