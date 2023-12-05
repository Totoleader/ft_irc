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
		disconnect_user(client_i);
		return ;
	}
	if (!strncmp(buf, "JOIN", 4))
		joinChannel(_users[client_i - 1], buf);
	if (!strncmp(buf, "PART", 4))
		leaveChannel(_users[client_i - 1], buf);

	std::cout << std::endl << "client send: " << buf << std::endl;


	parse_user_info(client_i, buf);
	for (size_t i = 1; i < fds.size(); i++)
	{
		if (i != (size_t)client_i)
			send(fds[i].fd, buf, strlen(buf), 0);
	}
	_users[client_i - 1].msgReceived();
}

void Server::leaveChannel(User &u, std::string msg)
{
	size_t	trail = msg.find("\r\n");

	std::string reply = u.getID() + " " + msg.substr(0, trail) + "\n";
	send(u.getFd(), reply.c_str(), reply.length(), 0);
}

void Server::joinChannel(User &u, std::string msg)
{
	size_t		hash = msg.find('#');
	size_t		trail = msg.find("\r\n");
	std::string	chan = msg.substr(0, trail).substr(hash);
	std::string join = u.getID() + " JOIN " + chan + "\n";
	std::string mode = ":127.0.0.1 MODE " + u.getNick() + " " + chan + " +nt\n";
	std::string listbegin = ":127.0.0.1 353 " + u.getNick() + " = " + chan + " :@" + u.getNick() + "\n";
	std::string listend = ":127.0.0.1 366 " + u.getNick() + " " + chan + " :End of /NAMES list.\n";

	std::cout << join << std::endl << mode << std::endl << listbegin << std::endl << listend << std::endl;

	send(u.getFd(), join.c_str(), join.length(), 0);
	send(u.getFd(), mode.c_str(), mode.length(), 0);
	send(u.getFd(), listbegin.c_str(), listbegin.length(), 0);
	send(u.getFd(), listend.c_str(), listend.length(), 0);

	//Si le channel n'existe pas encore
	std::map<std::string, Channel>::iterator it = _channels.find(chan);
	if (it == _channels.end())
	{
		
		Channel newChannel(chan, u);
		_channels[chan] = newChannel;
	}
	else//doit check si invite mode only et si user est whitelisted
	{
		_channels[chan].addUser(u);
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
		std::cout << "password received:" << buf << std::endl;
	
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

void Server::disconnect_user(int client_i)
{
	char buf[100];

	close(fds[client_i].fd);
	strcpy(buf, "User disconnected.");
	fds.erase(fds.begin() + client_i);
	_users.erase(_users.begin() + client_i - 1);
	std::cout << std::endl << "client send: " << buf << std::endl;
}

void Server::parse_user_info(int client_i, char *buf)
{
	std::string parseUserInfo = buf;
	if (!strncmp(buf, "NICK", 4))
		_users[client_i - 1].parseNickInfo(parseUserInfo);
	if (!strncmp(buf, "USER", 4))
		_users[client_i - 1].parseUserInfo(parseUserInfo);
	
	User *u = &_users[client_i - 1];
	if (!u->isConnected() && !u->getNick().empty() && !u->getUser().empty())
	{
		std::cout << u->getNick() << " is now connected." << std::endl;
		u->setConnected(true);
		connectClient(u);
	}
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
	std::string msg001 = ":127.0.0.1 001 " + u->getNick() + " :Welcome to the Internet Relay Network\n";
	std::string msg002 = ":127.0.0.1 002 " + u->getNick() + " :Your host is ft_irc, running version 0.1\n";
	std::string msg003 = ":127.0.0.1 003 " + u->getNick() + " :This server was created NOW\n";
	std::string msg004 = ":127.0.0.1 004 " + u->getNick() + " :ft_irc 0.1 * +i+t+k+o+l\n";

	send(u->getFd(), msg001.c_str(), msg001.length(), 0);
	send(u->getFd(), msg002.c_str(), msg002.length(), 0);
	send(u->getFd(), msg003.c_str(), msg003.length(), 0);
	send(u->getFd(), msg004.c_str(), msg004.length(), 0);
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

