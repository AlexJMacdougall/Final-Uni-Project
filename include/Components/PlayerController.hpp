#pragma once

#include "Components/ScriptComponent.hpp"
#include "Registry.hpp"
#include "SystemManager.hpp"

using Entity = uint32_t;

class PlayerController : public Script
{
public:
	PlayerController::PlayerController(Entity player, Camera2D* cameraPtr, Registry* registryPtr,SystemManager* systemPtr);

	void PlayerController::update(float dt) override;
private:
	Entity m_Player;
	Camera2D* m_cameraPtr;
	Registry* m_RegistryPtr;
	SystemManager* m_SystemPtr;

	float m_speed;
};

