#include "raylib.h"

#include "Registry.hpp"

using Entity = std::uint32_t;

class System {
public:
	virtual void Run();
	void AddEntities(std::set<Entity> newEntities);
	std::set<Entity> GetEntities();
private:
	std::set<Entity> m_Entities = {};
};

class DrawSystem : public System {
public:
	DrawSystem::DrawSystem(std::shared_ptr<Registry> registryPtr);
	void Run() override;
private:
	std::shared_ptr<Registry> m_RegistryPtr;
};