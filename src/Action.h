#pragma once

#include <string>

class Action
{
	std::string m_Name;
	std::string m_Type;

public:
	Action(const std::string& name, const std::string& type);

	const std::string& name() const;
	const std::string& type() const;
};

