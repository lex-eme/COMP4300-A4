#pragma once

#include "Scene.h"

class Scene_Zelda : public Scene
{
	struct PlayerConfig
	{
		float X, Y, CX, CY, SPEED, HEALTH;
	};

	std::string					m_LevelPath;
	PlayerConfig				m_PlayerConfig;
	bool						m_DrawTextures = true;
	bool						m_DrawCollision = false;
	bool						m_DrawGrid = false;
	bool						m_DrawDebug = false;
	bool						m_Follow = false;
	const Vec2					m_GridSize = { 64.0f, 64.0f };
	sf::Text					m_GridText;

public:
	Scene_Zelda(GameEngine* gameEngine, const std::string& levelPath);

private:
	void init(const std::string& levelPath);
	void loadLevel(const std::string& filename);
	void onEnd();
	void update() override;
	void spawnPlayer();
	void spawnSword(std::shared_ptr<Entity> entity);
	Vec2 getPosition(int rx, int ry, int tx, int ty) const;
	std::shared_ptr<Entity> player();

	// Systems
	void sMovement();
	void sAI();
	void sStatus();
	void sAnimation();
	void sCollision();
	void sCamera();
	void sGUI();
	void sRender() override;
	void sDoAction(const Action& action) override;

	// Helper
	void moveEntities(const std::string& tag);
};

