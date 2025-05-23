#pragma once

#include <set>
#include <map>
#include <array>
#include <string>

#include <raylib.h>

#include "Components/StructComponents.hpp"
#include "Components/ScriptComponent.hpp"
#include "Components/Vec2.hpp"
#include "Registry.hpp"

using Entity = uint32_t;

const int NUM_OF_ROOM_TEMPLATES = 2;

const std::array<RoomTemplate, NUM_OF_ROOM_TEMPLATES> ROOM_TEMPLATES = {
	RoomTemplate
	{
		{
			{1,1,1,1,1,1,4,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1},
			{2,0,0,0,0,0,0,0,0,0,0,0,3},
			{1,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,5,1,1,1,1,1,1}
		}
	},
	
	RoomTemplate
	{
		{
			{1,1,1,1,1,1,1,1,4,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
			{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,1,5,1,1,1,1,1,1,1,1}
		}
	}
};

class LevelManager
{
public:
	LevelManager::LevelManager(Registry* registrPtr);

	void LevelManager::GenerateLevel(int steps);
	Vec2 LevelManager::GetCurrentPos();
	void LevelManager::Move(std::string dir);
	void SetPlayer(Entity player);

	bool LevelManager::CheckForRoom(Vec2 pos);
	int LevelManager::GetRoomID(Vec2 pos);

	void LoadCurrentRoom();

	std::set<Entity> GetCurrentRoomDoorEntities();

	float loadedRoom = false;
private:
	Registry* m_RegistryPtr;

	Entity m_PlayerEntity;

	//Vector of room positions and IDs
	std::vector<std::pair<Vec2,int>> m_Rooms;
	//Current room position
	Vec2 m_CurrentPos;
	//Set of all entities that make up current room - used for deletion later
	std::set<Entity> m_CurrentRoomEntities;
	//Contains the current room's door entities - used to check if they have been interacted with
	std::set<Entity> m_CurrentRoomDoorEntities;

	std::map<std::string, Vec2> directionVectors = { 
		{"Down",Vec2{0,1}},  
		{"Left",Vec2{-1,0}}, 
		{"Right",Vec2{1,0}},  
		{"Up",Vec2{0,-1}}  
	};

	void LevelManager::Build(int id, int x, int y);
};

using Entity = std::uint32_t;

class DoorScript : public Script
{
public:
	DoorScript(Entity entity, Entity player, Registry* registryPtr, LevelManager* levelPtr, std::string dir);

	void update(float dt) override;
private:
	Entity m_Entity;
	Entity m_PlayerEntity;
	Registry* m_RegistryPtr;
	LevelManager* m_LevelPtr;
	std::string m_Direction;
	float m_InteractRange;

	float GetDistance(Entity entity1, Entity entity2);
};