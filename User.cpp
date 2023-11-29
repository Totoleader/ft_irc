#include "User.hpp"

User::User()
{

}

User::~User()
{

}

bool User::checkNameValidity(std::string const & name)
{
	if (name.empty())
		return false;
	for (char c : name)
	{
		if (!std::isprint(c) || c == ' ')
			return false;
	}
	return true;
}