#include "User.hpp"

User::User()
{
}

User::~User()
{

}

// bool User::recieveInfo(int fd)
// {
// 	char		buf[1024];
// 	std::string	nick_msg;
// 	std::string	user_msg;

// 	memset(buf, 0, sizeof buf);
// 	recv(fd, buf, sizeof buf, 0);
// 	if (!strncmp(buf, "NICK", 4))
// 	{
// 		nick_msg.append(buf, strlen(buf));
// 		parseNickInfo(nick_msg);
// 	}
// 	else if (!strncmp(buf, "USER", 4))
// 	{
// 		user_msg.append(buf, strlen(buf));
// 		parseUserInfo(user_msg);
// 	}
// 	return true;
// }

// Will recieve NICK Sam
void User::parseNickInfo(std::string nick_msg)
{
	this->_nick = nick_msg.substr(6);
}

// Will recieve USER scloutie 0 * Samuel
void User::parseUserInfo(std::string nick_msg)
{
	this->_login_name = nick_msg.substr(6);
}

struct sockaddr_storage	*User::getSock() const
{
	return (_sock);
}

int User::getFd() const
{
	return (_fd);
}

void User::setFd(int fd)
{
	_fd = fd;
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