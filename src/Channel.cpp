#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(string name, User &u, string password): _name(name), _password(password)
{
	addUser(u);
	addModerator(u);
}

Channel::~Channel()
{
}

void Channel::addUser(User &user)
{
	_channelUsers[user.getNick()] = user;
}

void Channel::addModerator(User &mod)
{
	_operatorList.push_back(mod);
}

void Channel::giveOp(User &giver, User &reveiver)
{
	if (isOperator(giver))
	{
		addModerator(reveiver);
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

std::vector<User>& Channel::getOperatorList()
{
	return _operatorList;
}

bool Channel::isOperator(User &u)
{
	for (std::vector<User>::iterator it = _operatorList.begin(); it != _operatorList.end(); it++)
	{
		if (u.getNick() == (*it).getNick())
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
	for (std::vector<User>::iterator it = _inviteList.begin(); it != _inviteList.end(); it++)
	{
		if (u.getNick() == (*it).getNick())
			return true;
	}
	return false;
}

string Channel::getPassword()
{
	return _password;
}

void	Channel::addToWhiteList(User &newUser)
{
	_inviteList.push_back(newUser);
}