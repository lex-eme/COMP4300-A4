#pragma once

#include <string>
#include <vector>

#include "Scene.h"

class Scene_Menu : public Scene
{
	std::string					m_Title;
	std::vector<std::string>	m_MenuStrings;
	std::vector<std::string>	m_LevelPaths;
	sf::Text					m_MenuText;
	size_t						m_MenuIndex = 0;

public:
	Scene_Menu(GameEngine* gameEngine = nullptr);

	void sRender() override;

private:
	void init();
	void update() override;
	void onEnd() override;

	// Systems
	void sDoAction(const Action& action) override;
};

