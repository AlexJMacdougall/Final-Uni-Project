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
	Vec2 UV;
	std::string SpriteSheetID;
	int Layer = 0; //Default layer
};

struct SpriteSheet {
	std::string ID;
	Texture textures;
	Vec2 size;
	float spriteSize;
};

struct Animation
{
	Vec2 startRect;
	int frames;
	int currentFrame = 0;
};

struct AnimatedSprite {
	Sprite* entitySprite;
	float frameTime;
	float currentFrameTime = 0;
	std::map<std::string, Animation> animationData;
	std::string currentAnimation;
	std::string lastAnimationFrame;
};

