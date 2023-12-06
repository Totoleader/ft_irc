#include "User.hpp"

User::User():_first_msg(true)
{
	_connected = false;
}

User::~User()
{

}

void User::setIp()
{
	struct sockaddr_in *addrin = (struct sockaddr_in*)(_sock);
	struct in_addr addr = addrin->sin_addr;

	_ip = inet_ntoa(addr);
	_port = std::to_string(addrin->sin_port);
}

// Will recieve NICK Sam
// or			NICK :Sam sam -> invalide
void User::parseNickInfo(std::string nick_msg)
{
	size_t	col = nick_msg.find(":");
	if (col != std::string::npos)
	{
		std::cout << "spaces not allowed" << std::endl;
		return ; // send error msg to client 
	}

	size_t	space = nick_msg.find(" ");

	_nick = nick_msg.substr(space + 1);
}

// Will recieve USER scloutie 0 * Samuel
// or			USER :s cloutie 0 * :Samuel C -> bad
void User::parseUserInfo(std::string nick_msg)
{
    size_t space1Pos = nick_msg.find(' ');
    size_t space2Pos = nick_msg.find(' ', space1Pos + 1);
	size_t last = nick_msg.find("* :");
	
	if (last == std::string::npos)
		last = nick_msg.find_last_of(' ');
	else
		last = last + 2;

	_login_name = nick_msg.substr(space1Pos + 1, space2Pos - space1Pos - 1);
	_real_name = nick_msg.substr(last + 1);
}

std::string User::getIp()
{
	return _ip;
}

std::string User::getPort()
{
	return _port;
}

std::string User::getNick()
{
	return _nick;
}

std::string User::getUser()
{
	return _login_name;
}

std::string User::getName()
{
	return _real_name;
}

std::string User::getID()
{
	return ":" + getNick() + "!" + getUser() + "@127.0.0.1:" + getPort();
}

struct sockaddr_storage	*User::getSock() const
{
	return (_sock);
}

void User::setSock(struct sockaddr_storage *s)
{
	_sock = s;
}

int User::getFd() const
{
	return (_fd);
}

void User::setFd(int fd)
{
	_fd = fd;
}

bool User::isConnected()
{
	return _connected;
}

void User::setConnected(bool is_connected)
{
	_connected = is_connected;
}

void User::setBuffer(char *buf)
{
	buffer += buf;
}

std::string User::getBuffer() const
{
	return (buffer);
}

void User::doneWithCommandGoToNextPlz(size_t *trail)
{
	buffer = buffer.substr(*trail + 2);
	*trail = getBuffer().find("\r\n");
	msgReceived();
}

bool	User::isFirstMsg() const
{
	return (_first_msg);
}

void	User::msgReceived()
{
	if (_first_msg == true)
		_first_msg = false;
}

// bool User::checkNameValidity(std::string const & name)
// {
// 	if (name.empty())
// 		return false;
// 	for (char c : name)
// 	{
// 		if (!std::isprint(c) || c == ' ')
// 			return false;
// 	}
// 	return true;
// }