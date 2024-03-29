#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "Vec2.h"

class Animation
{
	sf::Sprite		m_Sprite;
	size_t			m_FrameCount = 1; // total number of frames of animation
	size_t			m_CurrentFrame = 0; // the currentframe of animation being played
	size_t			m_Speed = 0; // the speed to play this animation
	Vec2			m_Size; // size of the animation frame
	std::string		m_Name = "none";

public:
	Animation();
	Animation(const std::string& name, const sf::Texture& t);
	Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed);

	void update();
	bool hasEnded() const;
	const std::string& getName() const;
	const Vec2& getSize() const;
	sf::Sprite& getSprite();
};

