#include "Vec2.hpp"

Vec2 Vec2Add(Vec2 v1, Vec2 v2)
{
    Vec2 result = { v1.x + v2.x, v1.y + v2.y };
    return result;
}

bool CompareVec2(Vec2 v1, Vec2 v2)
{
    return(v1.x == v2.x && v1.y == v2.y);
}
