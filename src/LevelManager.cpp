#include "LevelManager.hpp"

#include<iostream>
#include<cstdlib>
#include<cassert>

LevelManager::LevelManager()
{
	m_CurrentPos = { 0,0 };
}

void LevelManager::GenerateLevel(int targetRoomNum)
{
	//Clear old room data
	m_Rooms = {};

	Vec2 currentPos = {0,0};
	Vec2 newPos;

	srand(time(NULL));

	//Create starting room, always template 0
	m_Rooms.push_back(RoomTemplate{ 0,currentPos });
	int numOfRooms = 1;

	while(numOfRooms<targetRoomNum)
	{
		//Randomly select direction to move in
		std::string connectionDir[4] = {"Up","Down","Left","Right"};

		//Update position
		newPos = Vec2Add(currentPos, directionVectors[connectionDir[rand() % 4]]);

		//If there is no room at the pos, add a new one
		if(!CheckForRoom(newPos))
		{
			int roomID = rand() % NUM_OF_ROOM_TEMPLATES;
			m_Rooms.push_back(RoomTemplate{roomID,newPos});
			numOfRooms += 1;
		}

		//Step to new position
		currentPos = newPos;
	}

	m_CurrentPos = { 0,0 };
}

Vec2 LevelManager::GetCurrentPos()
{
	return m_CurrentPos;
}

void LevelManager::SetPlayer(Entity player)
{
	m_PlayerEntity = player;
}

void LevelManager::SetReigstryPtr(Registry* registryPtr)
{
	m_RegistryPtr = registryPtr;
}

bool LevelManager::CheckForRoom(Vec2 pos)
{
	//Checks for a room at a given position
	for (auto room : m_Rooms)
	{
		if (CompareVec2(pos, room.position))
		{
			return true;
		}
	}
	return false;
}

void LevelManager::Move(std::string dir)
{
	//Check there is a room in the direction passed in
	if (CheckForRoom(Vec2Add(m_CurrentPos, directionVectors[dir])))
	{
		m_CurrentPos = Vec2Add(m_CurrentPos, directionVectors[dir]);
	}
	else
	{
		std::cout << "You tried to move in direction (" << directionVectors[dir].x << ", " << directionVectors[dir].y << ") to position (" << Vec2Add(m_CurrentPos, directionVectors[dir]).x << ", " << Vec2Add(m_CurrentPos, directionVectors[dir]).y << ") but no room exists" << std::endl;
	}
}

int LevelManager::GetRoomID(Vec2 pos)
{
	//Check that a room exists at the position passed in
	//If you failed this assertion you tried to get a room at a position where one does not exist
	assert(CheckForRoom(pos));

	for (auto room : m_Rooms)
	{
		if (CompareVec2(room.position, pos))
		{
			return room.templateID;
		}
	}
}

void LevelManager::LoadCurrentRoom()
{
	//Clear old room entities
	for (Entity entity : m_CurrentRoomEntities) 
	{ 
		m_RegistryPtr->DestroyEntity(entity); 
	}
	m_CurrentRoomEntities = {};
	m_CurrentRoomDoorEntities = {};

	auto roomTextureMap = ROOM_TEMPLATES[GetRoomID(m_CurrentPos)];

	for(int x=0; x < roomTextureMap.size();x++)
	{
		for (int y=0; y < roomTextureMap[x].size();y++)
		{
			Build(roomTextureMap[x][y], x, y,Vec2{(float)roomTextureMap.size(),(float)roomTextureMap[x].size()});
		}
	}
}

std::set<Entity> LevelManager::GetDoorEntities()
{
	return m_CurrentRoomDoorEntities;
}

void LevelManager::Build(int id, int x, int y,Vec2 size)
{
	Entity newEntity = m_RegistryPtr->CreateEntity();
	m_RegistryPtr->AddComponent<Transform2D>(newEntity, Transform2D{ {(float)x * 32,(float)y * 32},{1,1} });
	
	std::string direction;

	m_CurrentRoomEntities.insert(newEntity);
	
	switch (id) {
	case(0): //Blank Space
		break;

	case(1): //Floor
		m_RegistryPtr->AddComponent<Sprite>(newEntity, { {0,1},"LevelSprites" });
		m_RegistryPtr->AddComponent<Navmesh>(newEntity, { Vec2{(float)x,(float)y} });
		break;

	case(2)://Wall
		m_RegistryPtr->AddComponent<Sprite>(newEntity, { {0,0},"LevelSprites" });
		m_RegistryPtr->AddComponent<BoxCollider>(newEntity, BoxCollider{ 32,32 });
		break;

	case(3)://Door
		m_RegistryPtr->DestroyEntity(newEntity);
		m_CurrentRoomEntities.erase(newEntity);
		SpawnDoor(x,y,size);
		break;

	case(4): //Enemy Spawn

		Build(1, x, y, size);

		m_RegistryPtr->DestroyEntity(newEntity);
		m_CurrentRoomEntities.erase(newEntity);

		m_CurrentRoomEntities.insert(SpawnEnemy(x,y));

		break;

	default:
		m_RegistryPtr->DestroyEntity(newEntity);
		m_CurrentRoomEntities.erase(newEntity);
	}
}

Entity LevelManager::SpawnEnemy(int x, int y)
{
	Entity enemy = m_RegistryPtr->CreateEntity();

	m_RegistryPtr->AddComponent<Transform2D>(enemy, Transform2D{ Vec2{300,300},Vec2{1,1} });
	m_RegistryPtr->AddComponent<BoxCollider>(enemy, BoxCollider{ 32.0f,32.0f });

	Sprite enemySprite = Sprite{ {0,0},"Enemy",1 };
	m_RegistryPtr->AddComponent<Sprite>(enemy, enemySprite);
	AnimatedSprite enemyAnimations = AnimatedSprite{ &enemySprite,0.2f };

	enemyAnimations.animationData["Idle"] = Animation{ Vec2{0,0},5 };
	enemyAnimations.animationData["Move"] = Animation{ Vec2{0,1},5 };
	enemyAnimations.animationData["Windup"] = Animation{ Vec2{0,2},3 };
	enemyAnimations.animationData["Attack"] = Animation{ Vec2{3,2},3 };
	enemyAnimations.animationData["Recovery"] = Animation{ Vec2{1,3},3 };

	m_RegistryPtr->AddComponent<AnimatedSprite>(enemy, enemyAnimations);

	m_RegistryPtr->AddComponent<ScriptComponent>(enemy, ScriptComponent());
	m_RegistryPtr->GetComponent<ScriptComponent>(enemy)->attachScript<MeleeEnemyScript>(MeleeEnemyScript(enemy, m_RegistryPtr, m_PlayerEntity));

	return enemy;
}

void LevelManager::SpawnDoor(int x, int y,Vec2 size)
{
	std::string direction;

	//Find which direction this door is
	if (x == 0) { direction = "Left"; }
	else if (x == size.x - 1) { direction = "Right"; }
	else if (y == 0) { direction = "Up"; }
	else if (y == size.y - 1) { direction = "Down"; }

	if (CheckForRoom(Vec2Add(directionVectors[direction], m_CurrentPos)))
	{
		Entity newDoor = m_RegistryPtr->CreateEntity();
		m_RegistryPtr->AddComponent<Transform2D>(newDoor, Transform2D{ {(float)x * 32,(float)y * 32},{1,1} });

		m_RegistryPtr->AddComponent<Sprite>(newDoor, { {0,1},"LevelSprites" });

		//DOOR SCRIPTCOMPONENTS MESSING WITH PLAYER SCRIPTCOMPONENTS

		DoorScript *doorScript = new DoorScript(newDoor, m_RegistryPtr, m_PlayerEntity, direction);
		ScriptComponent doorComp = ScriptComponent();
		doorComp.attachScript<DoorScript>(*doorScript);

		m_RegistryPtr->AddComponent<ScriptComponent>(newDoor, doorComp);

		m_CurrentRoomEntities.insert(newDoor);
		m_CurrentRoomDoorEntities.insert(newDoor);
	}
	else
	{
		Build(2, x, y, size);
	}
	std::cout << "Leaving scope for door" << std::endl;
}
