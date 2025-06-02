#pragma once

#include "Components/ScriptComponent.hpp"
#include "Registry.hpp"

using Entity = uint32_t;

class SpellWordScript : public Script
{
public:
	SpellWordScript(Entity attachedEntity, Registry* registryPtr,Entity Player);
	void update(float dt) override;
	void SetActive(bool val);
private:
	bool m_Active = false;
	Entity m_PlayerEntity;
};