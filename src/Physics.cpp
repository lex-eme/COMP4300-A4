#include "Physics.h"

static Vec2 overlap(const Vec2& p1, const Vec2& p2, const Vec2& s1, const Vec2& s2)
{
	float overlapX = s2.x + s1.x - abs(p2.x - p1.x);
	float overlapY = s2.y + s1.y - abs(p2.y - p1.y);

	return Vec2(overlapX, overlapY);
}

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2)
{
	const auto& p1 = e1->get<CTransform>().pos;
	const auto& p2 = e2->get<CTransform>().pos;
	const auto& bb1 = e1->get<CBoundingBox>().halfSize;
	const auto& bb2 = e2->get<CBoundingBox>().halfSize;
	return overlap(p1, p2, bb1, bb2);
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2)
{
	const auto& p1 = e1->get<CTransform>().prevPos;
	const auto& p2 = e2->get<CTransform>().prevPos;
	const auto& bb1 = e1->get<CBoundingBox>().halfSize;
	const auto& bb2 = e2->get<CBoundingBox>().halfSize;
	return overlap(p1, p2, bb1, bb2);
}

bool Physics::IsInside(const Vec2& pos, std::shared_ptr<Entity> e)
{
	return false;
}

Intersect Physics::LineIntersect(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d)
{
	return { false, Vec2() };
}

bool Physics::EntityIntersect(const Vec2& a, const Vec2& b, std::shared_ptr<Entity> e)
{
	return false;
}
