#pragma once

#include <unordered_map>
#include <memory>

#include "EntityManager.h"
#include "Action.h"

class GameEngine;

typedef std::unordered_map<int, std::string> ActionMap;

class Scene
{
protected:
	GameEngine*		m_Game = nullptr;
	EntityManager	m_EntityManager;
	ActionMap		m_ActionMap;
	bool			m_Paused = false;
	size_t			m_CurrentFrame = 0;

public:
	Scene();
	Scene(GameEngine* gameEngine);

	virtual void update() = 0;
	virtual void sDoAction(const Action& action) = 0;
	virtual void sRender() = 0;

	void simulate(const size_t frames);
	void registerAction(int inputKey, const std::string& actionName);

	size_t width() const;
	size_t height() const;
	size_t currentFrame() const;

	bool hasEnded() const {}
	const ActionMap& getActionMap() const;
	void drawLine(const Vec2& p1, const Vec2& p2);

private:
	virtual void onEnd() = 0;
};

