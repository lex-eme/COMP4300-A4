#include "Scene_Menu.h"

#include "GameEngine.h"
#include "Scene_Zelda.h"

Scene_Menu::Scene_Menu(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
}

void Scene_Menu::sRender()
{
	auto& window = m_Game->window();
	window.clear(sf::Color::Blue);

	for (size_t i = 0; i < m_MenuStrings.size(); i++)
	{
		m_MenuText.setString(m_MenuStrings[i]);
		m_MenuText.setPosition(30.0f, i * 60.0f + 40.0f);
		window.draw(m_MenuText);
	}
}

void Scene_Menu::init()
{
	registerAction(sf::Keyboard::Z, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::D, "PLAY");
	registerAction(sf::Keyboard::Q, "QUIT");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_MenuStrings.push_back("Level  1");
	m_MenuStrings.push_back("Level  2");
	m_MenuStrings.push_back("Level  3");

	m_LevelPaths.push_back("assets/levels/level1.txt");
	m_LevelPaths.push_back("assets/levels/level1.txt");
	m_LevelPaths.push_back("assets/levels/level1.txt");

	m_MenuText.setFont(m_Game->assets().getFont("Megaman"));
}

void Scene_Menu::update()
{
	m_EntityManager.update();
	m_Game->window().setView(m_Game->window().getDefaultView());
}

void Scene_Menu::onEnd()
{
	m_Game->quit();
}

void Scene_Menu::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "UP")
		{
			if (m_MenuIndex > 0) { m_MenuIndex -= 1; }
			else { m_MenuIndex = m_MenuStrings.size() - 1; }
		}
		else if (action.name() == "DOWN")
		{
			m_MenuIndex = (m_MenuIndex + 1) % m_MenuStrings.size();
		}
		else if (action.name() == "PLAY")
		{
			m_Game->changeScene("PLAY", std::make_shared<Scene_Zelda>(m_Game, m_LevelPaths[m_MenuIndex]));
		}
		else if (action.name() == "QUIT")
		{
			onEnd();
		}
	}
}
