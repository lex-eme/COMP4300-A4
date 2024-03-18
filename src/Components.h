#pragma once

#include "Vec2.h"
#include "Animation.h"
#include <SFML/Graphics.hpp>

struct Component
{
	bool has = false;
};

struct CTransform : public Component
{
	Vec2 pos		= { 0.0f, 0.0f };
	Vec2 prevPos	= { 0.0f, 0.0f };
	Vec2 scale		= { 1.0f, 1.0f };
	Vec2 velocity	= { 0.0f, 0.0f };
	Vec2 facing		= { 0.0f, 1.0f };
	float angle		= 0.0f;

	CTransform() {}
	CTransform(const Vec2& p)
		: pos(p), prevPos(p) {}
	CTransform(const Vec2& p, const Vec2& sp, const Vec2& sc, float a)
		: pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) {}
};

struct CLifeSpan : public Component
{
	int lifespan = 0;
	int frameCreated = 0;

	CLifeSpan() {}
	CLifeSpan(int duration, int frame)
		: lifespan(duration), frameCreated(frame) {}
};

struct CDamage : public Component
{
	int damage = 1;

	CDamage() {}
	CDamage(int d)
		: damage(d) {}
};

struct CInput : public Component
{
	bool up			= false;
	bool down		= false;
	bool left		= false;
	bool right		= false;
	bool attack		= false;

	CInput() {}
};

struct CBoundingBox : public Component
{
	Vec2 size;
	Vec2 halfSize;
	bool blockMove = false;
	bool blockVision = false;

	CBoundingBox() {}
	CBoundingBox(const Vec2& s)
		: size(s), halfSize(s.x / 2, s.y / 2) {}
	CBoundingBox(const Vec2& s, bool bm, bool bv)
		: size(s), halfSize(s.x / 2, s.y / 2), blockMove(bm), blockVision(bv) {}
};

struct CAnimation : public Component
{
	Animation animation;
	bool repeat = false;

	CAnimation() {}
	CAnimation(const Animation& anim, bool r)
		: animation(anim), repeat(r) {}
};

struct CState : public Component
{
	std::string state = "stand";

	CState() {}
	CState(const std::string& s)
		: state(s) {}
};

struct CInvincibility : public Component
{
	int iframes = 0;

	CInvincibility() {}
	CInvincibility(int f)
		: iframes(f) {}
};

struct CHealth : public Component
{
	int max = 1;
	int current = 1;

	CHealth() {}
	CHealth(int m)
		: max(m), current(m) {}
};

struct CPatrol : public Component
{
	std::vector<Vec2> positions;
	size_t currentPosition = 0;
	float speed = 0.0f;

	CPatrol() {}
	CPatrol(std::vector<Vec2>& pos, float s)
		: positions(pos), speed(s) {}
};

struct CFollowPlayer : public Component
{
	Vec2 home;
	float speed = 0.0f;

	CFollowPlayer() {}
	CFollowPlayer(Vec2 p, float s)
		: home(p), speed(s) {}
};