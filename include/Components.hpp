#pragma once

#include "raylib.h"

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