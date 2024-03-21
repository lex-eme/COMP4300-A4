#include "Scene.h"
#include "GameEngine.h"

Scene::Scene()
{
}

Scene::Scene(GameEngine* gameEngine)
	: m_Game(gameEngine)
{
}

void Scene::simulate(const size_t frames)
{
	for (size_t i = 0; i < frames; i++)
	{
		update();
	}
}

void Scene::registerAction(int inputKey, const std::string& actionName)
{
	m_ActionMap[inputKey] = actionName;
}

size_t Scene::width() const
{
	return m_Game->window().getSize().x;
}

size_t Scene::height() const
{
	return m_Game->window().getSize().y;
}

size_t Scene::currentFrame() const
{
	return m_CurrentFrame;
}

const ActionMap& Scene::getActionMap() const
{
	return m_ActionMap;
}

void Scene::drawLine(const Vec2& p1, const Vec2& p2)
{
	sf::Vertex line[] = { sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y) };
	m_Game->window().draw(line, 2, sf::Lines);
}
