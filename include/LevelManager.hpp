
#include <unordered_map>
#include <utility> //Pair

#include <raylib.h>

#include "Registry.hpp"
#include "Vec2.hpp"

using Entity = uint32_t;

const int NUM_OF_ROOM_TEMPLATES = 3;

class LevelManager
{
public:
	LevelManager::LevelManager(Registry* registrPtr);
	void LevelManager::GenerateLevel();
private:
	Registry* m_RegistryPtr;

	//Level Generation
	std::array<Color, NUM_OF_ROOM_TEMPLATES> ROOM_TEMPLATES = { RED,PURPLE,GREEN };
	std::array<Vec2, 4> directionVectors = { Vec2{0,1},Vec2{-1,0} ,Vec2{1,0} ,Vec2{0,-1} };
	//Set containing room entites
	std::set<Entity> m_Rooms;
	Vec2 currentPos;

	Entity LevelManager::GetRoomAtPos(Vec2 pos);
};