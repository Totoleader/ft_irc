#include "Server.hpp"

Server::Server():_sock(-1), _password("")
{
}

Server::Server(std::string password):_sock(-1), _password(password)
{
}

Server::~Server()
{
	freeaddrinfo(_servinfo);
}



void Server::init()
{
	addrinfo hints;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, "6667", &hints, &_servinfo);

	_sock = socket(_servinfo->ai_family, _servinfo->ai_socktype, _servinfo->ai_protocol);

	if (bind(_sock, _servinfo->ai_addr, _servinfo->ai_addrlen) != 0)
	{
		std::cerr << "Bind failed." << std::endl;
		return ;
	}

	listen(_sock, 10);
}

void Server::init_clients()
{
	int new_fd;
	struct sockaddr_storage client_info;
	socklen_t	addr_size = sizeof(client_info);
	

	while (1)
	{
		new_fd = accept(_sock, (struct sockaddr *)&client_info, &addr_size);
		handle_client(new_fd);//launché dans un thread
	}
}

void Server::handle_client(int new_fd)
{
	char buf[100];
	int password_is_ok = 0;

	memset(buf, 0, 100);
	recv(new_fd, buf, 100, 0);//lis le message et le met dans le buffer
	check_password(buf, &password_is_ok, new_fd);
	while (password_is_ok)
	{
		memset(buf, 0, 100);
		if (recv(new_fd, buf, 100, 0) < 1)
			break;
		std::cout << std::endl << "client send:" << buf << std::endl;
		send(new_fd, buf, strlen(buf), 0);//renvoie au client ce qui est dans le buffer
	}
}

void Server::check_password(char *buf, int *password_ok, int new_fd)
{
	const char *password = getPassword();
	
	if (*password == '\0')
	{
		*password_ok = true;
	}
	else if (!strncmp(buf, "PASS", 4))
	{
		std::cout << std::endl << "client send:" << buf << std::endl;
		if (!strncmp(buf + 5, password, strlen(password)) && *(buf + 5 + strlen(password) + 1) != '\r')
		{
			*password_ok = 1;
			std::cout << "right password" << std::endl;
		}
		else
		{
			std::cout << "wrong password" << std::endl;
			close(new_fd);
		}
		send(new_fd, buf, strlen(buf), 0);//renvoie au client ce qui est dans le buffer
	}
}



void Server::setPassword(std::string newPassword)
{
	_password = newPassword;
}

const char* Server::getPassword() const
{
	return (_password.c_str());
}

int Server::getSocket() const
{
	return (_sock);
}
