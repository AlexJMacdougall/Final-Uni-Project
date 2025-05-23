#include "LevelManager.hpp"

#include<iostream>
#include<cstdlib>
#include<cassert>

LevelManager::LevelManager(Registry* registryPtr):
	m_RegistryPtr(registryPtr)
{
	m_CurrentPos = { 0,0 };
}

void LevelManager::GenerateLevel(int steps)
{
	//Clear old level entites
	m_Rooms = {};

	Entity currentRoom;
	Entity newRoom;

	Vec2 currentPos = {0,0};
	Vec2 newPos;
	int connectionDir;

	srand(time(NULL));

	//Create starting room, always template 0
	m_Rooms.push_back(std::pair{ currentPos, 0 });

	std::cout << "Position: " << currentPos.x << " " << currentPos.y << " ID: " << 0 << std::endl;

	for(int i = 0;i<steps;i++)
	{
		//Randomly select direction to move in
		std::string connectionDir[4] = {"Up","Down","Left","Right"};

		//Update position
		newPos = Vec2Add(currentPos, directionVectors[connectionDir[rand() % 4]]);

		//If there is no room at the pos, add a new one

		if(!CheckForRoom(newPos))
		{
			int roomID = rand() % NUM_OF_ROOM_TEMPLATES;
			m_Rooms.push_back({ newPos,roomID });
			std::cout << "Position: " << newPos.x << " " << newPos.y << " ID: " << roomID << std::endl;
		}

		//Step to new position
		currentPos = newPos;
	}
}

Vec2 LevelManager::GetCurrentPos()
{
	return m_CurrentPos;
}

void LevelManager::Move(std::string dir)
{
	//Check there is a room in the direction passed in
	std::cout << "Moving in direction: " << directionVectors[dir].x << ", " << directionVectors[dir].y <<"(" <<dir<<")"<< std::endl;
	if(CheckForRoom(Vec2Add(m_CurrentPos, directionVectors[dir])))
	{
		m_CurrentPos = Vec2Add(m_CurrentPos, directionVectors[dir]);
		std::cout << m_CurrentPos.x << " " << m_CurrentPos.y << std::endl;
		loadedRoom = false;
	}
	else
	{
		std::cout << "You tried to move in direction (" << directionVectors[dir].x << ", " << directionVectors[dir].y << ") to position (" << Vec2Add(m_CurrentPos, directionVectors[dir]).x << ", "<< Vec2Add(m_CurrentPos, directionVectors[dir]).y<<") but no room exists" << std::endl;
	}
}

void LevelManager::SetPlayer(Entity player)
{
	m_PlayerEntity = player;
}

bool LevelManager::CheckForRoom(Vec2 pos)
{
	//Checks for a room at a given position
	for (auto room : m_Rooms)
	{
		if (CompareVec2(pos, room.first))
		{
			return true;
		}
	}
	return false;
}

int LevelManager::GetRoomID(Vec2 pos)
{
	//Check that a room exists at the position passed in
	//If you failed this assertion you tried to get a room at a position where one does not exist
	assert(CheckForRoom(pos));

	for (auto room : m_Rooms)
	{
		if (CompareVec2(pos, room.first))
		{
			return room.second;
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

	auto roomTextureMap = ROOM_TEMPLATES[GetRoomID(m_CurrentPos)].entityMap;

	for(int x=0; x < roomTextureMap.size();x++)
	{
		for (int y=0; y < roomTextureMap[x].size();y++)
		{
			Build(roomTextureMap[x][y], x, y);
		}
	}
}

std::set<Entity> LevelManager::GetCurrentRoomDoorEntities()
{
	return m_CurrentRoomDoorEntities;
}

void LevelManager::Build(int id, int x, int y)
{
	Entity newEntity = m_RegistryPtr->CreateEntity();
	m_RegistryPtr->AddComponent<Transform>(newEntity, Transform{ {(float)x * 32,(float)y * 32,0},Quaternion{0},{1,1,0} });

	m_CurrentRoomEntities.insert(newEntity);
	
	switch (id) {
	case(0): //Floor
		m_RegistryPtr->AddComponent<Sprite>(newEntity, { {0,32,32,32},0 });
		break;

	case(1): //Wall
		m_RegistryPtr->AddComponent<Sprite>(newEntity, {{0,0,32,32},0});
		m_RegistryPtr->AddComponent<BoxCollider>(newEntity, BoxCollider{ 32,32 });
		break;

	case(2)://Up Door
		if (CheckForRoom(Vec2Add(directionVectors["Up"], m_CurrentPos)))
		{
			m_RegistryPtr->AddComponent<Sprite>(newEntity, { {0,0,32,32},1,1 });

			DoorScript doorScript = DoorScript(newEntity, m_PlayerEntity, m_RegistryPtr,this, "Up");
			ScriptComponent doorComp = ScriptComponent();
			doorComp.attachScript<DoorScript>(doorScript);

			m_RegistryPtr->AddComponent<ScriptComponent>(newEntity, doorComp);

			m_CurrentRoomDoorEntities.insert(newEntity);
		}
		else
		{
			m_RegistryPtr->AddComponent<Sprite>(newEntity, { {0,0,32,32},0 });
			m_RegistryPtr->AddComponent<BoxCollider>(newEntity, BoxCollider{ 32,32 });
		}
		break;

	case(3)://Down Door
		if (CheckForRoom(Vec2Add(directionVectors["Down"], m_CurrentPos)))
		{
			m_RegistryPtr->AddComponent<Sprite>(newEntity, { {0,0,32,32},1,1 });

			DoorScript doorScript = DoorScript(newEntity, m_PlayerEntity, m_RegistryPtr,this, "Down");
			ScriptComponent doorComp = ScriptComponent();
			doorComp.attachScript<DoorScript>(doorScript);

			m_RegistryPtr->AddComponent<ScriptComponent>(newEntity, doorComp);

			m_CurrentRoomDoorEntities.insert(newEntity);
		}
		else
		{
			m_RegistryPtr->AddComponent<Sprite>(newEntity, { {0,0,32,32},0 });
			m_RegistryPtr->AddComponent<BoxCollider>(newEntity, BoxCollider{ 32,32 });
		}
		break;

	case(4)://Right Door
		if (CheckForRoom(Vec2Add(directionVectors["Left"], m_CurrentPos)))
		{
			m_RegistryPtr->AddComponent<Sprite>(newEntity, { {0,0,32,32},1,1 });

			DoorScript doorScript = DoorScript(newEntity, m_PlayerEntity, m_RegistryPtr,this, "Left");
			ScriptComponent doorComp = ScriptComponent();
			doorComp.attachScript<DoorScript>(doorScript);

			m_RegistryPtr->AddComponent<ScriptComponent>(newEntity, doorComp);

			m_CurrentRoomDoorEntities.insert(newEntity);
		}
		else
		{
			m_RegistryPtr->AddComponent<Sprite>(newEntity, { {0,0,32,32},0 });
			m_RegistryPtr->AddComponent<BoxCollider>(newEntity, BoxCollider{ 32,32 });
		}
		break;

	case(5)://Left Door
		if (CheckForRoom(Vec2Add(directionVectors["Right"], m_CurrentPos)))
		{
			m_RegistryPtr->AddComponent<Sprite>(newEntity, { {0,0,32,32},1,1 });

			DoorScript doorScript = DoorScript(newEntity, m_PlayerEntity, m_RegistryPtr,this, "Right");
			ScriptComponent doorComp = ScriptComponent();
			doorComp.attachScript<DoorScript>(doorScript);

			m_RegistryPtr->AddComponent<ScriptComponent>(newEntity, doorComp);

			m_CurrentRoomDoorEntities.insert(newEntity);
		}
		else
		{
			m_RegistryPtr->AddComponent<Sprite>(newEntity, { {0,0,32,32},0 });
			m_RegistryPtr->AddComponent<BoxCollider>(newEntity, BoxCollider{ 32,32 });
		}
		break;

	default:
		m_RegistryPtr->DestroyEntity(newEntity);
		m_CurrentRoomEntities.erase(newEntity);
	}
}
