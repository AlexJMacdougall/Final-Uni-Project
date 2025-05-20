#pragma once

#include "raylib.h"
#include "Vec2.hpp"

//Small amout of custom structs

struct Physics
{
	Vector3 acceleration = { 0,0 };
	Vector3 velocity = { 0,0 };
};

struct SphereCollider
{
	float radius;
};

struct BoxCollider
{
	float width;
	float height;
};

struct Room {
	int id;
	Vec2 position;
	std::array<Entity*, 4> connections;
};

struct RoomTemplate {
	//std::array<Vec2, 4> doorPositions;
	std::vector<std::vector<int>> textureMap;
};

struct Sprite
{
	Rectangle textureRect;
	int SpriteSheetID;
};