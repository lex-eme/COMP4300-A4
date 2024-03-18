#include "Assets.h"

#include <iostream>
#include <fstream>

void Assets::loadFromFile(const std::string& path)
{
	std::ifstream fin(path);
	std::string type;

	while (fin >> type)
	{
		if (type == "Texture")
		{
			std::string name;
			std::string path;
			fin >> name >> path;
			addTexture(name, path);
		}
		else if (type == "Animation")
		{
			std::string name;
			std::string textureName;
			size_t frameCount = 0, speed = 0;

			fin >> name >> textureName >> frameCount >> speed;

			const sf::Texture& texture = getTexture(textureName);
			Animation anim(name, texture, frameCount, speed);
			addAnimation(name, anim);
		}
		else if (type == "Font")
		{
			std::string name;
			std::string path;

			fin >> name >> path;

			addFont(name, path);
		}
	}
}

void Assets::addTexture(const std::string& name, const std::string& path)
{
	if (m_Textures.find(name) != m_Textures.end())
	{
		std::cout << "Texture '" << name << "' already exists" << std::endl;
		return;
	}

	if (!m_Textures[name].loadFromFile(path))
	{
		std::cerr << "Could not load texture '" << name << "' from path '" << path << "'" << std::endl;
		m_Textures.erase(name);
	}
	else
	{
		std::cout << "Loaded Texture '" << name << "' from path '" << path << "'" << std::endl;
	}
}

void Assets::addAnimation(const std::string& name, Animation& anim)
{
	if (m_Animations.find(name) != m_Animations.end())
	{
		std::cout << "Animation '" << name << "' already exists" << std::endl;
		return;
	}

	m_Animations[name] = anim;
}

void Assets::addSound(const std::string& name, const std::string& path)
{
	if (m_SoundBuffers.find(name) != m_SoundBuffers.end())
	{
		std::cout << "SoundBuffer '" << name << "' already exists" << std::endl;
		return;
	}

	if (!m_SoundBuffers[name].loadFromFile(path))
	{
		std::cerr << "Could not load sound '" << name << "' from path '" << path << "'" << std::endl;
		m_SoundBuffers.erase(name);
	}
	else
	{
		std::cout << "Loaded Sound '" << name << "' from path '" << path << "'" << std::endl;
		m_Sounds[name].setBuffer(m_SoundBuffers[name]);
		m_Sounds[name].setVolume(25.0f);
	}
}

void Assets::addFont(const std::string& name, const std::string& path)
{
	if (m_Fonts.find(name) != m_Fonts.end())
	{
		std::cout << "Font '" << name << "' already exists" << std::endl;
		return;
	}

	if (!m_Fonts[name].loadFromFile(path))
	{
		std::cerr << "Could not load font '" << name << "' from path '" << path << "'" << std::endl;
		m_Fonts.erase(name);
	}
	else
	{
		std::cout << "Loaded Font '" << name << "' from path '" << path << "'" << std::endl;
	}
}

const sf::Texture& Assets::getTexture(const std::string& name) const
{
	auto it = m_Textures.find(name);
	if (it != m_Textures.end())
	{
		return it->second;
	}

	std::cerr << "Trying to use unknown texture called " << name << std::endl;
	exit(-1);
}

const Animation& Assets::getAnimation(const std::string& name) const
{
	auto it = m_Animations.find(name);
	if (it != m_Animations.end())
	{
		return it->second;
	}

	std::cerr << "Trying to use unknown animation called " << name << std::endl;
	exit(-1);
}

sf::Sound& Assets::getSound(const std::string& name)
{
	auto it = m_Sounds.find(name);
	if (it != m_Sounds.end())
	{
		return it->second;
	}

	std::cerr << "Trying to use unknown sound called " << name << std::endl;
	exit(-1);
}

const sf::Font& Assets::getFont(const std::string& name) const
{
	auto it = m_Fonts.find(name);
	if (it != m_Fonts.end())
	{
		return it->second;
	}

	std::cerr << "Trying to use unknown font called " << name << std::endl;
	exit(-1);
}
