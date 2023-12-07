#ifndef __USER_H_
# define __USER_H_

# include <string>
# include <iostream>
# include <netdb.h>
# include <arpa/inet.h>
# include <vector>
# include <Libs.hpp>

class User
{
private:
	string				_nick; 
	string				_login_name;
	string				_real_name;
	string				_ip;
	string				_port;
	bool					_connected;
	int						_fd;
	bool					_first_msg;
	struct sockaddr_storage	*_sock;

	string				buffer;
	
public:
	User();
	~User();

	bool	checkNameValidity(string const & name);
	bool	recieveInfo(int fd);
	void	parseNickInfo(string nick_msg);
	void	parseUserInfo(string user_msg);
	
	void	setIp();

	string	getNick();

	string	getUser();

	string	getName();

	string	getIp();

	string	getPort();

	string	getID();

	struct sockaddr_storage	*getSock() const;
	void					setSock(struct sockaddr_storage *s);

	void	setFd(int fd);
	int		getFd()const;
	bool	isFirstMsg() const;
	void	msgReceived();

	bool	isConnected();
	void	setConnected(bool is_connected);
	void	setBuffer(char *buf);
	string	getBuffer() const;
	void	doneWithCommandGoToNextPlz(size_t *trail);
};

#endif