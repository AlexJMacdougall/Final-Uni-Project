#pragma once

#include <vector>

#include <raylib.h>

#include "Components/Vec2.hpp"

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

struct RoomTemplate {
	std::vector<std::vector<int>> entityMap;
};

struct Sprite
{
	Rectangle textureRect;
	int SpriteSheetID;
	int Layer = 0; //Default layer
};