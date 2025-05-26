#pragma once

#include <vector>
#include <map>

#include <raylib.h>

#include "Components/Vec2.hpp"

struct Transform2D
{
	Vec2 position;
	Vec2 scale;
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
	int templateID;
	Vec2 position;
};

struct Sprite
{
	Rectangle textureRect;
	int SpriteSheetID;
	int Layer = 0; //Default layer
};

const int NUM_OF_ANIMATIONS = 4;

struct Animation
{
	Vec2 startRect;
	int frames;
};

struct AnimatedSprite {
	Sprite sprite;
	float frameTime;
	std::array<Animation, NUM_OF_ANIMATIONS> animationData;
};

