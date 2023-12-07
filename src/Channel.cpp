#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(string name, User &u, string password): _name(name), _password(password)
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

void Channel::addModerator(string modName)
{
	_moderatorName.push_back(modName);
}

string Channel::getName()
{
	return _name;
}

std::map <string, User> & Channel::getUsers()
{
	return _channelUsers;
}

std::vector<string>& Channel::getmoderatorName()
{
	return _moderatorName;
}

bool Channel::isOperator(User &u)
{
	for (std::vector<string>::iterator it = _moderatorName.begin(); it != _moderatorName.end(); it++)
	{
		if (u.getNick() == *it)
			return true;
	}
	return false;
}

bool Channel::isInviteOnly()
{
	return (_isInviteOnly);
}

bool Channel::isWhitelisted(User &u)
{
	for (std::vector<string>::iterator it = _inviteList.begin(); it != _inviteList.end(); it++)
	{
		if (u.getNick() == *it)
			return true;
	}
	return false;
}

string Channel::getPassword()
{
	return _password;
}