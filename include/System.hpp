#include "raylib.h"

#include "Registry.hpp"

using Entity = std::uint32_t;

class System {
public:
	System::System();
	virtual void Run(float dt);
	void AddEntities(std::set<Entity> newEntities);
	std::set<Entity> GetEntities();
private:
	std::set<Entity> m_Entities = {};
};

class DrawSystem : public System {
public:
	DrawSystem::DrawSystem(std::shared_ptr<Registry> registryPtr);
	void Run(float dt) override;
private:
	std::shared_ptr<Registry> m_RegistryPtr;
};

class PlayerController : public System {
public:
	PlayerController::PlayerController(std::shared_ptr<Registry> registryPtr);
	void Run(float dt) override;
private:
	std::shared_ptr<Registry> m_RegistryPtr;

	int m_HorizontalMovement = 0;
	int m_VerticalMovement = 0;

	float speed = 10;
};