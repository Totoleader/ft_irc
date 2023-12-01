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
	int poll_events;

	while (1)
	{
		poll_events = poll(fds.data(), fds.size(), -1);
	
		if (fds[0].revents & POLLIN)
		{
			new_client();
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

void Server::handle_client(int client_i)
{
	char buf[100];
	memset(buf, 0, 100);
	if (recv(fds[client_i].fd, buf, 100, 0) <= 0 || (_users[client_i - 1].isFirstMsg() && !check_password(buf)))
	{
		close(fds[client_i].fd);
		strcpy(buf, "User disconnected.");
		fds.erase(fds.begin() + client_i);
		_users.erase(_users.begin() + client_i - 1);
	}
	std::string parseUserInfo = buf;
	if (!strncmp(buf, "NICK", 4))
		_users[client_i - 1].parseNickInfo(parseUserInfo);
	if (!strncmp(buf, "USER", 4))
		_users[client_i - 1].parseUserInfo(parseUserInfo);

	std::cout << std::endl << "client send: " << buf << std::endl;
	for (size_t i = 1; i < fds.size(); i++)
	{
		if (i != (size_t)client_i)
			send(fds[i].fd, buf, strlen(buf), 0);
	}
	_users[client_i - 1].msgReceived();
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
	return false;
}

User *Server::getUser(int fd)
{
	for (unsigned int i = 0; i < _users.size(); i++)
	{
		if (fd == _users[i].getFd())
			return &_users[i];
	}
	return NULL;
}

void Server::new_client()
{
	struct pollfd	newClient;
	struct sockaddr_storage cl;
	User			newUser;
	int				new_fd;
	
	socklen_t	addr_size = sizeof cl;
	new_fd = accept(fds[0].fd, (struct sockaddr *)&cl, &addr_size);
	fcntl(new_fd, F_SETFL, O_NONBLOCK);

	newUser.setSock(&cl);
	newUser.setFd(new_fd);
	newUser.setIp();

	_users.push_back(newUser);
	
	newClient.events = POLLIN;
	newClient.fd = new_fd;
	fds.push_back(newClient);
}

void Server::new_client(int fd)
{
	struct pollfd	newClient;
	// User			newUser;
	
	fcntl(fd, F_SETFL, O_NONBLOCK);

	// newUser.setFd(fd);
	// _users.push_back(newUser);
	newClient.events = POLLIN;
	newClient.fd = fd;
	fds.push_back(newClient);
}

void Server::connectClient(User *u)
{
	std::string msg;

	msg = ":127.0.0.1:6667 001 " + u->getNick() + " :Welcome to the Internet Relay Network\n";

	send(u->getFd(), msg.c_str(), msg.length(), 0);
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

