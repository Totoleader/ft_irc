#include "Server.hpp"
#include "Libs.hpp"

Server::Server():_password("")
{
}

Server::Server(std::string password):_password(password)
{
}

Server::~Server()
{
	freeaddrinfo(_servinfo);
}



void Server::init()
{
	addrinfo hints;
	int servSocket;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, "6667", &hints, &_servinfo);

	servSocket = socket(_servinfo->ai_family, _servinfo->ai_socktype, _servinfo->ai_protocol);
	new_client(servSocket);

	if (bind(fds[0].fd, _servinfo->ai_addr, _servinfo->ai_addrlen) != 0)
	{
		std::cerr << "Bind failed." << std::endl;
		return ;
	}

	listen(fds[0].fd, 10);//!!!10 = max connection
}

void Server::init_clients()
{
	int new_fd;
	struct sockaddr_storage client_info;
	socklen_t	addr_size = sizeof(client_info);
	int poll_events;

	while (1)
	{
		poll_events = poll(fds.data(), fds.size(), -1);
		
		if (fds[0].revents & POLLIN)
		{
			new_fd = accept(fds[0].fd, (struct sockaddr *)&client_info, &addr_size);
			new_client(new_fd);
			poll_events--;
		}
		for (size_t i = 1; i < fds.size() && poll_events; i++)
		{
			if (fds[i].revents & POLLIN)
			{
				handle_client(i);
				poll_events--;
			}
		}
		
	}
}

void Server::handle_client(int revent_i)
{
	// int password_is_ok = 0;
	// memset(buf, 0, 100);
	// recv(revent_fd, buf, 100, 0);//lis le message et le met dans le buffer
	// check_password(buf, &password_is_ok, revent_fd);

	char buf[100];
	memset(buf, 0, 100);
	if (recv(fds[revent_i].fd, buf, 100, 0) <= 0 || !check_password(buf))
	{
		close(fds[revent_i].fd);
		strcpy(buf, "User disconnected.");
		fds.erase(fds.begin() + revent_i);
	}

	std::cout << std::endl << "client send: " << buf << std::endl;
	for (size_t i = 1; i < fds.size(); i++)
	{
		if (i != (size_t)revent_i)
			send(fds[i].fd, buf, strlen(buf), 0);
	}
}

bool Server::check_password(char *buf)
{
	const char *password = getPassword();

	if (*password == '\0')
	{
		return true;
	}
	else if (!strncmp(buf, "PASS", 4))
	{
		if (!strncmp(buf + 5, password, strlen(password)) && *(buf + 5 + strlen(password) + 1) != '\r')
		{
			std::cout << "right password" << std::endl;
			return true;
		}
		else
		{
			std::cout << "wrong password" << std::endl;
			return false;
		}
	}
	return true;
}

void Server::new_client(int fd)
{
	struct pollfd newClient;
	// char *buf[100];

	fcntl(fd, F_SETFL, O_NONBLOCK);
	newClient.events = POLL_IN;
	newClient.fd = fd;
	fds.push_back(newClient);

	// memset(buf, 0, 100);
	// recv(fd, buf, 100, 0);
	// check_password(buf, )
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
	return (fds[0].fd);
}
