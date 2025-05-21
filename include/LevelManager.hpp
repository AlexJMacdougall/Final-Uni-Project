#pragma once

#include <set>
#include <map>

#include <raylib.h>

#include "Registry.hpp"
#include "Vec2.hpp"

using Entity = uint32_t;

const int NUM_OF_ROOM_TEMPLATES = 2;

const std::array<RoomTemplate, NUM_OF_ROOM_TEMPLATES> ROOM_TEMPLATES = {
	RoomTemplate
	{
		{
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
		}
	},
	RoomTemplate
	{
		{
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
		}
	}
};

class LevelManager
{
public:
	LevelManager::LevelManager(Registry* registrPtr);

	void LevelManager::GenerateLevel(int steps);
	Vec2 LevelManager::GetCurrentPos();
	void LevelManager::Move(int dir);

	bool LevelManager::CheckForRoom(Vec2 pos);
	int LevelManager::GetRoomID(Vec2 pos);

	void LoadCurrentRoom();

private:
	Registry* m_RegistryPtr;

	//Vector of room positions and IDs
	std::vector<std::pair<Vec2,int>> m_Rooms;
	//Current room position
	Vec2 m_CurrentPos;
	//Set of all entities that make up current room - used for deletion later
	std::set<Entity> m_CurrentRoomEntities;

	std::array<Vec2, 4> directionVectors = { Vec2{0,1},Vec2{-1,0} ,Vec2{1,0} ,Vec2{0,-1} };

	void LevelManager::Build(int id, int x, int y);
};