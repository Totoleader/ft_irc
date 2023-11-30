#ifndef __SERVEUR_H__
#define __SERVEUR_H__

#include "Libs.hpp"

class Server
{
private:
	std::vector<struct pollfd>	fds;
	std::string			_password;
	addrinfo			*_servinfo;
	// std::vector<User>	_users;

	bool check_password(char *buf);
	void new_client(int fd);

public:
	Server();
	Server(std::string password);
	~Server();
	
	void init();
	void init_clients();
	void handle_client(int new_fd);


	void 		setPassword(std::string newPassword);
	const char	*getPassword()	const;
	int			getSocket()		const;
};

#endif
