#pragma once

#include <memory>
#include <vector>
#include <array>

#include <raylib.h>

#include "LevelManager.hpp"
#include "Registry.hpp"
#include "Components/ScriptComponent.hpp"
#include "Components/DoorScript.hpp"

using Entity = std::uint32_t;

const int NUM_OF_SPRITESHEETS = 2;
const int NUM_OF_LAYERS = 3;

class SystemManager
{
public:
	SystemManager(Registry* registryPtr,LevelManager* LEVEL, int screenWidth, int screenHeight);
	~SystemManager();

	void SetPlayer(Entity target);
	void Update(float dt);
	void RunScripts(float dt);
	void Draw();
	//void SystemManager::PlayerInput(float dt);
	float GetDistance(Entity entity1, Entity entity2);

	float GetSlowdownValue();
	void SetSlowdownValue(float slowdown);

	Camera2D* GetCamera();

	template<typename t>
	std::set<Entity> CheckCollision(Entity entity);
private:
	Registry* m_RegistryPtr;
	LevelManager* m_LevelManagerPtr;

	//Player Movement Variables
	float m_Speed = 2;
	float m_Slowdown = 1.0;

	Camera2D camera = { 0 };
	Entity m_PlayerEntity;
	int screenWidth = 1280;
	int screenHeight = 720;

	std::map<std::string, Vec2> directionVectors = {
	{"Down",Vec2{0,1}},
	{"Left",Vec2{-1,0}},
	{"Right",Vec2{1,0}},
	{"Up",Vec2{0,-1}}
	};
	std::map<std::string, std::string> directionOpposites = {
	{"Down","Up"},
	{"Left","Right"},
	{"Right","Left"},
	{"Up","Down"}
	};

	//SpriteSheets
	Texture LevelSprites;
	Texture PlayerSprites;

	//Map with pointers to Spritesheets, allow sprites to access them
	std::array<Texture*,NUM_OF_SPRITESHEETS> m_SpriteSheets;
};

template<typename t>
inline std::set<Entity> SystemManager::CheckCollision(Entity checkEntity)
{
	//Set to return
	std::set<Entity> collidingEntities = {};

	//Entities with two different collision types
	std::set<Entity> sphereColliderEntities = m_RegistryPtr->GetEntitiesWithComponent<SphereCollider>();
	std::set<Entity> boxColliderEntities = m_RegistryPtr->GetEntitiesWithComponent<BoxCollider>();

	//Remove entity being checked from sets of entities to check
	sphereColliderEntities.erase(checkEntity);
	boxColliderEntities.erase(checkEntity);

	if (typeid(t) == typeid(SphereCollider))
	{
		//Sphere to sphere collision
		for (Entity colliderEntity : sphereColliderEntities)
		{
			auto collider1 = m_RegistryPtr->GetComponent<SphereCollider>(checkEntity);
			auto collider2 = m_RegistryPtr->GetComponent<SphereCollider>(colliderEntity);

			float distance = GetDistance(checkEntity, colliderEntity);

			if ((distance - collider1->radius - collider2->radius) <= 0)
			{
				collidingEntities.insert(colliderEntity);
			}
		}
	}
	else if (typeid(t) == typeid(BoxCollider))
	{
		//Box to box collision
		for (Entity colliderEntity : boxColliderEntities)
		{
			auto pos1 = m_RegistryPtr->GetComponent<Transform2D>(checkEntity)->position;
			auto pos2 = m_RegistryPtr->GetComponent<Transform2D>(colliderEntity)->position;

			auto collider1 = m_RegistryPtr->GetComponent<BoxCollider>(checkEntity);
			auto collider2 = m_RegistryPtr->GetComponent<BoxCollider>(colliderEntity);

			//AABB positions for comparison
			float right1 = pos1.x + (collider1->width / 2);
			float left1 = pos1.x - (collider1->width / 2);

			float right2 = pos2.x + (collider2->width / 2);
			float left2 = pos2.x - (collider2->width / 2);

			float upper1 = pos1.y - (collider1->height / 2);
			float lower1 = pos1.y + (collider1->height / 2);

			float upper2 = pos2.y - (collider2->height / 2);
			float lower2 = pos2.y + (collider2->height / 2);

			if ((right1 >= left2) &&
				(left1 <= right2) &&
				(upper1 <= lower2) &&
				(lower1 >= upper2))
			{
				collidingEntities.insert(colliderEntity);
			}
		}
	}
	return collidingEntities;
}
