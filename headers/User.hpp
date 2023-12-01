#ifndef __USER_H_
# define __USER_H_

# include <string>
# include <iostream>
# include <netdb.h>

class User
{
private:
	std::string				_nick;
	std::string				_login_name;
	std::string				_real_name;
	bool					op;
	int						fd;
	struct sockaddr_storage	*sock;
	
public:
	User(int fd);
	~User();

	bool	checkNameValidity(std::string const & name);
	// bool	recieveInfo(int fd);
	void	parseNickInfo(std::string nick_msg);
	void	parseUserInfo(std::string user_msg);
	
};

#endif