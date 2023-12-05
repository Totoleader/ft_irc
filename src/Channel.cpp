#include "./headers/Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(std::string name, User &u): _name(name)
{
	addUser(u);
	addModerator(u.getName());
}

Channel::~Channel()
{
}

void Channel::addUser(User &user)
{
	_channelUsers[user.getName()] = user;
}

void Channel::addModerator(std::string modName)
{
	_moderatorName.push_back(modName);
}
