
#include <iostream>

#include "Components/PlayerController.hpp"

PlayerController::PlayerController(Entity player, Camera2D* cameraPtr, Registry* registryPtr, SystemManager* systemPtr) :
	Script()
{
	m_Player = player;
	m_cameraPtr = cameraPtr;
	m_RegistryPtr = registryPtr;
	m_SystemPtr = systemPtr;
	m_speed = 2;
}

void PlayerController::update(float dt)
{
	auto transform = m_RegistryPtr->GetComponent<Transform2D>(m_Player);

	int directionX = 0, directionY = 0;

	//Check for keypresses
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A))
	{
		directionX = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
	}
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
	{
		directionY = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
	}
	
	if (IsKeyPressed(KEY_Q))
	{
		m_SystemPtr->SetSlowdownValue(0.5);
	}
	else if (IsKeyReleased(KEY_Q))
	{
		m_SystemPtr->SetSlowdownValue(1.0);
	}

	//Move entity then check for collision
	
	transform->position.x += directionX * m_speed * m_SystemPtr->GetSlowdownValue();
	if (!(m_SystemPtr->CheckCollision<BoxCollider>(m_Player).empty())) { transform->position.x -= directionX * m_speed; }
	transform->position.y += directionY * m_speed * m_SystemPtr->GetSlowdownValue();
	if (!(m_SystemPtr->CheckCollision<BoxCollider>(m_Player).empty())) { transform->position.y -= directionY * m_speed; }
	
	//Update camera pos
	//auto cameraPos = m_RegistryPtr->GetComponent<Transform2D>(m_Player);
	m_cameraPtr->target = Vector2{ transform->position.x , transform->position.y };
}