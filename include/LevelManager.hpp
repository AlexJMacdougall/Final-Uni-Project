#pragma once

#include <set>

#include <raylib.h>

#include "Registry.hpp"
#include "Vec2.hpp"

using Entity = uint32_t;

const int NUM_OF_ROOM_TEMPLATES = 3;
const std::array<Color, NUM_OF_ROOM_TEMPLATES> ROOM_TEMPLATES = { RED,PURPLE,GREEN };

class LevelManager
{
public:
	LevelManager::LevelManager(Registry* registrPtr);
	LevelManager::~LevelManager();
	void LevelManager::GenerateLevel(int steps);

	std::set<Entity>* GetRooms();

private:
	Registry* m_RegistryPtr;

	//Set containing room entites
	std::set<Entity> m_Rooms;

	Entity LevelManager::GetRoomAtPos(Vec2 pos);
};