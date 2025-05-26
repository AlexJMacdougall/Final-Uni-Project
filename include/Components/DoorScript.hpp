#pragma once

#include <cstdint>
#include <string>

#include "ScriptComponent.hpp"
#include "Components/StructComponents.hpp"
#include "Registry.hpp"

using Entity = std::uint32_t;

class DoorScript : public Script
{
public:
	DoorScript(Entity entity, Registry* registryPtr, Entity m_PlayerEntity, std::string dir);

	void update(float dt) override;
	bool playerHasInteracted();
	std::string GetDirection();
private:
	Entity m_PlayerEntity;
	std::string m_Direction;
	float m_InteractRange;
	bool m_PlayerInteracted = false;
};