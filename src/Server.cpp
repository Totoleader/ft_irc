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

void Server::handle_client(int revent_i)
{
	char buf[100];
	memset(buf, 0, 100);
	if (recv(fds[revent_i].fd, buf, 100, 0) <= 0 || !check_password(buf))
	{
		close(fds[revent_i].fd);
		strcpy(buf, "User disconnected.");
		fds.erase(fds.begin() + revent_i);
	}

	User *u = &_users[revent_i - 1];

	if (!strncmp(buf, "NICK", 4))
		_users[revent_i - 1].parseNickInfo(buf);
	if (!strncmp(buf, "USER", 4))
		_users[revent_i - 1].parseUserInfo(buf);
	if (!u->isConnected() && !u->getNick().empty() && !u->getUser().empty())
	{
		std::cout << "im in" << std::endl;
		u->setConnected(true);
		connectClient(u);
	}

	std::cout << std::endl << "from client: " << buf << std::endl;
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
	
	fcntl(fd, F_SETFL, O_NONBLOCK);

	newClient.events = POLLIN;
	newClient.fd = fd;
	fds.push_back(newClient);
}

void Server::connectClient(User *u)
{
	std::string msg;

	msg = ":127.0.0.1 001 " + u->getNick() + " :Welcome to the Internet Relay Network "
			+ u->getNick() + "!" + u->getUser() + "@" + "127.0.0.1";

	std::cout << msg << std::endl;

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

