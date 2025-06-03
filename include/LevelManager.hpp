#pragma once

#include <set>
#include <map>
#include <array>
#include <string>

#include <raylib.h>

#include "Components/StructComponents.hpp"
#include "Components/ScriptComponent.hpp"
#include "Scripts/MeleeEnemyScript.hpp"
#include "Scripts/DoorScript.hpp"
#include "Components/Vec2.hpp"
#include "Registry.hpp"

const int NUM_OF_ROOM_TEMPLATES = 2;

const std::array<std::vector<std::vector<int>>, NUM_OF_ROOM_TEMPLATES> ROOM_TEMPLATES = {
	std::vector<std::vector<int>>
	{
		{
			{2,2,2,2,2,2,3,2,2,2,2,2,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,2},
			{3,1,1,1,1,1,1,1,1,1,1,1,3},
			{2,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,2,2,2,2,2,3,2,2,2,2,2,2},
		}
	},
	std::vector<std::vector<int>>
	{
		{
			{2,2,2,2,2,2,2,2,3,2,2,2,2,2,2,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3},
			{2,1,1,1,1,1,1,2,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,2,1,1,1,1,1,1,1,2},
			{2,1,1,1,2,2,2,2,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,2,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,2,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,2,1,1,1,1,1,1,1,2},
			{3,1,1,4,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
			{2,2,2,2,2,2,2,2,3,2,2,2,2,2,2,2},
		}
	}
};

using Entity = uint32_t;

class LevelManager
{
public:
	LevelManager::LevelManager();

	void LevelManager::GenerateLevel(int steps);
	Vec2* LevelManager::GetCurrentPos();
	void LevelManager::Move(std::string dir);
	void SetPlayer(Entity player);
	void SetReigstryPtr(Registry* registryPtr);

	bool LevelManager::CheckForRoom(Vec2 pos);
	Entity LevelManager::GetRoomAtPos(Vec2 pos);

	void LoadCurrentRoom();

	std::set<Entity> GetDoorEntities();

private:
	Registry* m_RegistryPtr;

	Entity m_PlayerEntity;

	//Vector of room templates
	std::set<Entity> m_Rooms;
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

	void LevelManager::Build(int id, int x, int y,Vec2 size);
	Entity LevelManager::SpawnEnemy(int x,int y);
	void LevelManager::SpawnDoor(int x,int y,Vec2 size);
};

