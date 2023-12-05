#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(std::string name, User &u): _name(name)
{
	addUser(u);
	addModerator(u.getNick());
}

Channel::~Channel()
{
}

void Channel::addUser(User &user)
{
	_channelUsers[user.getNick()] = user;
}

void Channel::addModerator(std::string modName)
{
	_moderatorName.push_back(modName);
}

std::string Channel::getName()
{
	return _name;
}

std::map <std::string, User> & Channel::getUsers()
{
	return _channelUsers;
}

std::vector<std::string>& Channel::getmoderatorName()
{
	return _moderatorName;
}

bool Channel::isOperator(User &u)
{
	for (std::vector<std::string>::iterator it = _moderatorName.begin(); it != _moderatorName.end(); it++)
	{
		if (u.getNick() == *it)
			return true;
	}
	return false;
}
