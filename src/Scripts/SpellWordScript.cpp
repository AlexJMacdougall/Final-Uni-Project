#include "Scripts/SpellWordScript.hpp"

SpellWordScript::SpellWordScript(Entity attachedEntity, Registry* registryPtr,Entity player,Camera2D* camera,SpellWordData data) :
	Script(attachedEntity, registryPtr),
	m_PlayerEntity(player),
	m_CameraPtr(camera),
	m_WordData(data)
{}

void SpellWordScript::update(float dt)
{
	if (m_Active) 
	{
		m_RegistryPtr->GetComponent<Sprite>(m_AttachedEntity)->hide = false;

		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			auto pos = m_RegistryPtr->GetComponent<Transform2D>(m_AttachedEntity)->position;
			auto rad = m_RegistryPtr->GetComponent<SphereCollider>(m_AttachedEntity)->radius;

			Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), *m_CameraPtr);

			if(GetDistance(pos,Vec2{ mousePos.x,mousePos.y }) < rad)
			{
				m_RegistryPtr->GetComponent<ScriptComponent>(m_PlayerEntity)->GetScript<PlayerController>()->AddSpellWord(m_AttachedEntity);
			}
		}
		else if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			auto pos = m_RegistryPtr->GetComponent<Transform2D>(m_AttachedEntity)->position;
			auto rad = m_RegistryPtr->GetComponent<SphereCollider>(m_AttachedEntity)->radius;

			Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), *m_CameraPtr);

			if (GetDistance(pos, Vec2{ mousePos.x,mousePos.y }) < rad)
			{
				m_RegistryPtr->GetComponent<ScriptComponent>(m_PlayerEntity)->GetScript<PlayerController>()->RemoveSpellWord(m_AttachedEntity);
			}
		}
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

SpellWordData* SpellWordScript::GetData()
{
	return &m_WordData;
}
