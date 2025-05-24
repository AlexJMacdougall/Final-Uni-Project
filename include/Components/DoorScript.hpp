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
	DoorScript(Entity entity, Entity player, Registry* registryPtr, std::string* interactDirection, std::string dir);

	void update(float dt) override;
private:
	Entity m_Entity;
	Entity m_PlayerEntity;
	Registry* m_RegistryPtr;
	std::string* m_InteractDirection;
	std::string m_Direction;
	float m_InteractRange;

	float GetDistance(Entity entity1, Entity entity2);
};