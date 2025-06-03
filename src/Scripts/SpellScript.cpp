#include "Scripts/SpellScript.hpp"

SpellScript::SpellScript(Entity entity, Registry* registryPtr, Vec2 dir):
	Script(entity,registryPtr),
	m_dir(dir)
{
	m_Speed = 50.0f;
}

void SpellScript::update(float dt)
{
	auto pos = m_RegistryPtr->GetComponent<Transform2D>(m_AttachedEntity)->position;

	std::cout << Vec2Add(pos, Vec2MultiplyFloat(m_dir, dt)).x << " " << Vec2Add(pos, Vec2MultiplyFloat(m_dir, dt)).y << std::endl;

	m_RegistryPtr->GetComponent<Transform2D>(m_AttachedEntity)->position = Vec2Add(pos, Vec2MultiplyFloat(m_dir, dt * m_Speed));
}
