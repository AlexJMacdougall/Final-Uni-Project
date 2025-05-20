#include "LevelManager.hpp"

#include<iostream>
#include<cstdlib>

LevelManager::LevelManager(Registry* registryPtr)
{
	m_RegistryPtr = registryPtr;
	m_RegistryPtr->RegisterComponent<Room>();
}

LevelManager::~LevelManager()
{
	for (Entity room : m_Rooms) { m_RegistryPtr->DestroyEntity(room); }
}

void LevelManager::GenerateLevel(int steps)
{
	//Clear old level entites
	for (Entity room : m_Rooms) { m_RegistryPtr->DestroyEntity(room); }

	Entity currentRoom;
	Entity newRoom;

	Vec2 currentPos = {0,0};
	Vec2 newPos;
	int connectionDir;

	std::array<Vec2, 4> directionVectors = { Vec2{0,1},Vec2{-1,0} ,Vec2{1,0} ,Vec2{0,-1} };

	Entity startRoom = m_RegistryPtr->CreateEntity();
	m_RegistryPtr->AddComponent<Room>(startRoom, Room{ 100.0f,50.0f,BLUE,currentPos });
	m_Rooms.insert(startRoom);

	currentRoom = startRoom;

	std::cout << "Created start room, id: " << startRoom << std::endl;

	for(int i = 0;i<steps;i++)
	{
		std::cout << "Start of loop, currentRoom: " << currentRoom << " CurrentPos: " << currentPos.x << " " << currentPos.y << std::endl;
		//Randomly select direction to move in
		connectionDir = rand() % 4;
		std::cout << "Connectiondir: " << connectionDir << std::endl;

		//Update position
		std::cout << "Adding currentPos, " << currentPos.x << " " << currentPos.y << ", to directionVectors at connectionDir(" << connectionDir << "), " << directionVectors[connectionDir].x << " " << directionVectors[connectionDir].y << std::endl;
		newPos = Vec2Add(currentPos, directionVectors[connectionDir]);

		std::cout << "newPos: " << newPos.x<<" "<<newPos.y << std::endl;
		std::cout << "Calling GetRoomAtPos() " << std::endl;
		//Get or create room at new coordinates
		newRoom = GetRoomAtPos(newPos);

		//Pass pointer to new Room to current room
		m_RegistryPtr->GetComponent<Room>(currentRoom)->connections[connectionDir] = &newRoom;

		std::cout << "Set currentroom(id " << currentRoom<<"), connection "<<connectionDir<< " as pointer to newRoom(id "<< newRoom <<")" << std::endl;

		//Pass pointer to current room to new room
		m_RegistryPtr->GetComponent<Room>(newRoom)->connections[3 - connectionDir] = &currentRoom;

		std::cout << "Set newRoom(id " << newRoom << "), connection " << 3-connectionDir << " as pointer to currentRoom(id " << currentRoom << ")" << std::endl;

		//Set new position and room to current
		std::cout << "Updating currentRoom and currentPos" << std::endl;
		currentRoom = newRoom;
		currentPos = newPos;
	}
}

std::set<Entity>* LevelManager::GetRooms()
{
	return &m_Rooms;
}

Entity LevelManager::GetRoomAtPos(Vec2 pos)
{
	//Checks for a room at a given position
	for(Entity room:m_Rooms)
	{
		if(CompareVec2(m_RegistryPtr->GetComponent<Room>(room)->position, pos))
		{
			std::cout << "Room " << room << " exists, returning" << std::endl;
			return room;
		}
	}
	//If there is no room there it creates one
	Entity newRoom = m_RegistryPtr->CreateEntity();
	m_RegistryPtr->AddComponent<Room>(newRoom, Room{ 100.0f,50.0f,ORANGE,pos });
	m_Rooms.insert(newRoom);
	std::cout << "Made new room: " << newRoom << std::endl;
	return newRoom;
}
