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
	int layer = 0;
	bool checkPerFrame = false; //If true, collision is checked each frame. Should set to true in constructor of scripts that need collision data
};

struct BoxCollider
{
	float width;
	float height;
	int layer = 0;
	bool checkPerFrame = false; //If true, collision is checked each frame. Should set to true in constructor of scripts that need collision data
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
	bool hide = false;
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
	bool finishedAnimation = false;
};

struct Navmesh {
	Vec2 pos;
};

struct SpellWordData
{
	std::string type;
	int power;
};