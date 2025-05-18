#include "raylib.h"

#include "Registry.hpp"
#include "Components.hpp"
/*
class System {
public:
	System::System(std::shared_ptr<Registry> registryPtr);
	virtual void Run(float dt);
	//void AddEntities(std::set<Entity> newEntities);
protected:
	//std::set<Entity> m_Entities = {};
	std::shared_ptr<Registry> m_RegistryPtr;
};

class DrawSystem : public System {
public:
	DrawSystem::DrawSystem(std::shared_ptr<Registry> registryPtr) :System(registryPtr) {};
	void Run(float dt) override;
};

class PlayerController : public System {
public:
	PlayerController::PlayerController(std::shared_ptr<Registry> registryPtr) :System(registryPtr) {};
	void Run(float dt) override;
private:
	int m_HorizontalMovement = 0;
	int m_VerticalMovement = 0;

	float speed = 100;
};

class PhysicsSystem : public System {
public:
	PhysicsSystem::PhysicsSystem(std::shared_ptr<Registry> registryPtr) :System(registryPtr) {};
	void Run(float dt) override;
private:
	float friction = 10.0f;
};

class CollisionSystem : public System {
public:
	CollisionSystem::CollisionSystem(std::shared_ptr<Registry> registryPtr) :System(registryPtr) {};
	void Run(float dt) override;
private:
	float friction = 10.0f;
};
*/