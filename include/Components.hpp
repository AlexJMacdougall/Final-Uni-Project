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
	float width;
	float height;
	Color colour;
	Vec2 position;
	std::array<Entity*,4> connections;
};