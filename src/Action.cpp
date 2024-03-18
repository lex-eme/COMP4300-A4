#include "Action.h"

Action::Action(const std::string& name, const std::string& type)
	: m_Name(name), m_Type(type)
{
}

const std::string& Action::name() const
{
	return m_Name;
}

const std::string& Action::type() const
{
	return m_Type;
}

