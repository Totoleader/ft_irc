#ifndef __SERVEUR_H__
#define __SERVEUR_H__

#include "Libs.hpp"

class Server
{
private:
	int				_sock;
	std::string		_password;
	addrinfo		*_servinfo;

	void check_password(char *buf, int *password_ok, int new_fd);

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

#endif // __SERVEUR_H__
