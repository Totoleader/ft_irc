#ifndef __SERVEUR_H__
#define __SERVEUR_H__

#include "Libs.hpp"
#include "User.hpp"

class Server
{
private:
	std::vector<struct pollfd>	fds;
	std::vector<User>			_users;
	std::string					_password;
	addrinfo					*_servinfo;

	bool check_password(char *buf);

public:
	Server();
	Server(std::string password);
	~Server();
	
	void init();
	void init_clients();
	void handle_client(int new_fd);

	void connectClient(User *u);

	void new_client();
	void new_client(int fd);

	User	*getUser(int fd);

	void 		setPassword(std::string newPassword);
	const char	*getPassword()	const;
	int			getSocket()		const;

};

#endif
