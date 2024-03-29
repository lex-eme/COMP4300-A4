#pragma once

#include "Entity.h"

struct Intersect
{
	bool result;
	Vec2 pos;
};

class Physics
{
public:
	static Vec2 GetOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2);
	static Vec2 GetPreviousOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2);
	static bool IsInside(const Vec2& pos, std::shared_ptr<Entity> e);
	static Intersect LineIntersect(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d);
	static bool EntityIntersect(const Vec2& a, const Vec2& b, std::shared_ptr<Entity> e);
};
