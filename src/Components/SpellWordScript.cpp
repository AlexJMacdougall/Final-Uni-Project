#include "Components/SpellWordScript.hpp"

SpellWordScript::SpellWordScript(Entity attachedEntity, Registry* registryPtr,Entity player) :
	Script(attachedEntity, registryPtr),
	m_PlayerEntity(player)
{}

void SpellWordScript::update(float dt)
{
	if (m_Active) 
	{
		m_RegistryPtr->GetComponent<Sprite>(m_AttachedEntity)->hide = false;
	}
	else
	{
		m_RegistryPtr->GetComponent<Sprite>(m_AttachedEntity)->hide = true;
	}
}

void SpellWordScript::SetActive(bool val)
{
	m_Active = val;
}
