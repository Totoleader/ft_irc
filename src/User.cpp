#include "User.hpp"

User::User(int fd)
{
	(void)fd;
	// recieveInfo(fd);
	// recieveInfo(fd);
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
// or			NICK :Sam sam -> invalide
void User::parseNickInfo(std::string nick_msg)
{
	size_t trail = nick_msg.find("\r\n");
	size_t	col = nick_msg.find(":");
	if (col != std::string::npos)
	{
		std::cout << "spaces not allowed" << std::endl;
		return 1;
	}

	size_t	space = nick_msg.find(" ");

	std::string nick = nick_msg.substr(0, trail).substr(space + 1);
	//save in instance
}

// Will recieve USER scloutie 0 * Samuel
// or			USER :s cloutie 0 * :Samuel C -> bad
void User::parseUserInfo(std::string nick_msg)
{
	size_t trail = nick_msg.find("\r\n");
    size_t space1Pos = nick_msg.find(' ');
    size_t space2Pos = nick_msg.find(' ', space1Pos + 1);
	size_t last = nick_msg.find("* :");
	
	if (last == std::string::npos)
		last = nick_msg.find_last_of(' ');
	else
		last = last + 2;

	std::string login = nick_msg.substr(space1Pos + 1, space2Pos - space1Pos - 1);
	std::string name = nick_msg.substr(0, trail).substr(last + 1);
	//save in instance
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