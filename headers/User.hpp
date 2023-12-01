#ifndef __USER_H_
# define __USER_H_

# include <string>
# include <iostream>
# include <netdb.h>
# include <arpa/inet.h>

class User
{
private:
	std::string				_nick;
	std::string				_login_name;
	std::string				_real_name;
	std::string				_ip;
	std::string				_port;
	bool					_connected;
	int						_fd;
	bool					_first_msg;
	struct sockaddr_storage	*_sock;
	
public:
	User();
	~User();

	bool	checkNameValidity(std::string const & name);
	bool	recieveInfo(int fd);
	void	parseNickInfo(std::string nick_msg);
	void	parseUserInfo(std::string user_msg);
	
	void	setIp();

	std::string	getNick();

	std::string	getUser();

	std::string	getName();

	struct sockaddr_storage	*getSock() const;
	void					setSock(struct sockaddr_storage *s);

	void	setFd(int fd);
	int		getFd()const;
	bool	isFirstMsg() const;
	void	msgReceived();
	int		getFd() const;

	bool	isConnected();
	void	setConnected(bool is_connected);
};

#endif