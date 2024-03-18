#include "Animation.h"

Animation::Animation()
{
}

Animation::Animation(const std::string& name, const sf::Texture& t)
	: Animation(name, t, 1, 0)
{
}

Animation::Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed)
	: m_Name(name),
	m_Sprite(t),
	m_FrameCount(frameCount),
	m_CurrentFrame(0),
	m_Speed(speed)
{
	m_Size = Vec2((float)(t.getSize().x / frameCount), (float)t.getSize().y);
	m_Sprite.setOrigin(m_Size.x / 2.0f, m_Size.y / 2.0f);
	m_Sprite.setTextureRect(sf::IntRect((int)std::floor(m_CurrentFrame * m_Size.x), 0, (int)m_Size.x, (int)m_Size.y));
}

void Animation::update()
{
	if (m_FrameCount == 1)
		return;

	m_CurrentFrame += 1;
	size_t frame = m_CurrentFrame / m_Speed;
	frame %= m_FrameCount;

	m_Sprite.setTextureRect(sf::IntRect(frame * (int)m_Size.x, 0, (int)m_Size.x, (int)m_Size.y));
}

const Vec2& Animation::getSize() const
{
	return m_Size;
}

const std::string& Animation::getName() const
{
	return m_Name;
}

sf::Sprite& Animation::getSprite()
{
	return m_Sprite;
}

bool Animation::hasEnded() const
{
	if (m_FrameCount == 1)
		return false;

	size_t frame = m_CurrentFrame / m_Speed;

	return frame > m_FrameCount;
}