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

void Channel::giveOp(User &giver, User &reveiver)
{
	if (isOperator(giver))
	{
		addModerator(reveiver.getNick());
		string opMsg = ":127.0.0.1 MODE " + _name + " +o " + _channelUsers.begin()->second.getNick() + "\r\n";
		// sendToChannel(chan, opMsg);
	}
	else
	{
		//could not give operator
	}
}

void Channel::sendToChannel(string message)
{
	for (std::map<string, User>::iterator it = _channelUsers.begin(); it != _channelUsers.end(); it++)
    {
    	send(it->second.getFd(), message.c_str(), message.length(), 0);
    }
}

void Channel::sendToChannelExcept(string message, User &except)
{
	for (std::map<string, User>::iterator it = _channelUsers.begin(); it != _channelUsers.end(); it++)
    {
		if (it->second.getNick() != except.getNick())
    		send(it->second.getFd(), message.c_str(), message.length(), 0);
    }
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

void Channel::setInviteOnly(bool state)
{
	_isInviteOnly = state;
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

void	Channel::addToWhiteList(string newUser)
{
	_inviteList.push_back(newUser);
}