#include "Server.hpp"
#include "Libs.hpp"

Server::Server():_password("")
{
}

Server::Server(string password):_password(password)
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

	hints.ai_family = AF_INET;
	memset(&hints, 0, sizeof hints); // memset bad
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, "6667", &hints, &_servinfo);

	servSocket = socket(_servinfo->ai_family, _servinfo->ai_socktype, _servinfo->ai_protocol);
	new_client(servSocket);

	if (bind(fds[0].fd, _servinfo->ai_addr, _servinfo->ai_addrlen) != 0)
	{
		std::cerr << "Bind failed." << endl;
		exit (1);
	}

	listen(fds[0].fd, 10);//!!!10 = max connection
}

void Server::init_clients()
{
	int poll_events;

	while (true)
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
	string command;
	size_t		trail;
	int i;

	i = client_i - 1;
	memset(buf, 0, 100); // memset bad
	
	if (recv(fds[client_i].fd, buf, 100, 0) <= 0)
		{disconnect_user(client_i); return ;}

	_users[i].setBuffer(buf);
	trail = _users[i].getBuffer().find("\r\n");
	while (trail != string::npos)
	{
		command = _users[i].getBuffer().substr(0, trail);

		if (_users[i].isFirstMsg() && !check_password(buf))
			{disconnect_user(client_i); return ;}
		if (command.substr(0, 4) == "JOIN")
			getAndJoinChannels(_users[i], command.substr(5)); //enleve la partie JOIN du message
		if (command.substr(0, 4) == "PART")
			leaveChannel(_users[i], command.substr(5));

		cout << endl << "client send: " << command.c_str() << endl;

		parse_user_info(client_i, command);
		// for (size_t i = 1; i < fds.size(); i++)
		// {
		// 	if (i != (size_t)client_i)
		// 		send(fds[i].fd, command.c_str(), strlen(command.c_str()), 0);
		// }
		_users[i].doneWithCommandGoToNextPlz(&trail);
	}
}

void Server::getAndJoinChannels(User &u, string channels_msg)
{
	std::map<string, string> chans = parseChannels(channels_msg);

	std::map<string, string>::iterator it;
	for (it = chans.begin(); it != chans.end(); it++)
	{
		// peut etre verifier le nom du channel ici
		// verifier le mot de passe
		joinChannel(u, *it);
	}
}

void Server::joinExistingChannel(User &u, Channel &chan)
{
	string	join = u.getID() + " JOIN " + chan.getName() + "\r\n";
	string listBegin = ":127.0.0.1 353 " + u.getNick() + " = " + chan.getName() + " :";
	string listEnd = ":127.0.0.1 366 " + u.getNick() + " " + chan.getName() + " :End of /NAMES list.\r\n";
	send(u.getFd(), join.c_str(), join.length(), 0);
	sendToChannelExcept(chan.getName(), join, u);
	for (std::map<string, User>::iterator it = chan.getUsers().begin(); it != chan.getUsers().end(); it++)
	{
		if (chan.isOperator(it->second))
			listBegin += "@";
		listBegin += it->second.getNick() + " ";
	}
	listBegin += "\r\n";
	cout << listBegin << endl;
	send(u.getFd(), listBegin.c_str(), listBegin.length(), 0);
	send(u.getFd(), listEnd.c_str(), listEnd.length(), 0);
}

void Server::leaveChannel(User &u, string msg)
{
	std::istringstream	stream(msg);
	string				token;
	std::vector<string>	args;
	int					in_word = false;

	// mettre ca dans fonction std::vector<string> getArgs(string msg)
	if (msg == ":")
	{
		cout << "bruh u need more args" << endl;
		return ;
	}
	while (std::getline(stream, token, ' '))
	{
		cout << "token: " << token << endl;
		if (token.at(0) == ':' && !in_word)
		{
			in_word = true;
			token = (token.length() > 1) ? token.substr(1) : "";
			args.push_back(token);
			continue ;
		}
		if (!in_word)
			args.push_back(token);
		else if (args.size() == 2)
			args[1] += " " + token;
	}

	
	string part_msg;
	if (args.size() == 1)
	{
		part_msg = u.getID() + " PART " + args[0] + "\r\n";
	}
	else if (args.size() == 2)
	{
		part_msg = u.getID() + " PART " + args[0] + " :" + args[1] + "\r\n";
	}
	cout << "to channel: " << part_msg << endl;

	std::string reply = u.getID() + " PART " + msg + "\r\n";
	send(u.getFd(), reply.c_str(), reply.length(), 0);

	
	sendToChannelExcept(args[0], part_msg, u);

	_channels[args[0]].getUsers().erase(u.getNick());


	if (_channels[args[0]].getUsers().size() <= 0)
	{
		_channels.erase(args[0]);
		return ;
	}

	std::vector<string>::iterator it = _channels[args[0]].getmoderatorName().begin();
	while (_channels[args[0]].getmoderatorName().size() == 1 && it != _channels[args[0]].getmoderatorName().end())
	{
		if (*it == u.getNick())
		{
			_channels[args[0]].getmoderatorName().erase(it);
			_channels[args[0]].getmoderatorName().push_back(_channels[args[0]].getUsers().begin()->first);

			string opMsg = ":127.0.0.1 MODE " + args[0] + " +o " + _channels[args[0]].getUsers().begin()->second.getNick() + "\r\n";
			sendToChannel(args[0], opMsg);
			break ;
		}
		it++;
	}
}

void Server::joinChannel(User &u, std::pair<string, string> chan)
{
	//Si le channel n'existe pas encore
	std::map<string, Channel>::iterator it = _channels.find(chan.first);
	if (it == _channels.end())//si le channel n'existe pas
	{
		cout << "*Creating channel*" << endl;
		createChannelMsg(u, chan.first);
		Channel newChannel(chan.first, u, chan.second);
		_channels[chan.first] = newChannel;
	}
	else if (!checkInvited(u, _channels[chan.first])) //bouncer
	{
		
	}
	else if (!passIsOk(_channels[chan.first], chan.second))
	{
		std::string err_badchankey = ":127.0.0.1 475 " + u.getNick() + " " + chan.first + " :Cannot join channel (+k)\r\n";
		send(u.getFd(), err_badchankey.c_str(), err_badchankey.length(), 0);
		return ;
	}
	else // join
	{
		_channels[chan.first].addUser(u);
		joinExistingChannel(u, _channels[chan.first]);
	}
}

bool Server::check_password(char *buf) //strncmp bad
{
	const char *password = getPassword();

	if (*password == '\0')
	{
		return true;
	}
	else if (!strncmp(buf, "PASS", 4))
	{
		cout << "password received:" << buf << endl;
	
		if (!strncmp(buf + 5, password, strlen(password)) && *(buf + 5 + strlen(password) + 1) != '\r')
		{
			cout << "right password" << endl;
			return true;
		}
		else
		{
			cout << "wrong password" << endl;
			return false;
		}
	}
	return false;
}

void Server::disconnect_user(int client_i)
{
	char buf[100];

	if ((size_t)client_i >= fds.size())
		return ;
	close(fds[client_i].fd);
	fds.erase(fds.begin() + client_i);
	_users.erase(_users.begin() + client_i - 1);
	strcpy(buf, "User disconnected."); //strcpy bad
	cout << endl << "client send: " << buf << client_i << ":" << fds.size() << endl;
}

void Server::parse_user_info(int client_i, string parseUserInfo)
{
	if (parseUserInfo.substr(0, 4) == "NICK")
		_users[client_i - 1].parseNickInfo(parseUserInfo);
	if (parseUserInfo.substr(0, 4) == "USER")
		_users[client_i - 1].parseUserInfo(parseUserInfo);

	User *u = &_users[client_i - 1];
	if (!u->isConnected() && !u->getNick().empty() && !u->getUser().empty())
	{
		cout << u->getNick() << " is now connected." << endl;
		u->setConnected(true);
		connectClient(u);
	}
}

void Server::createChannelMsg(User &u, string chan) const
{
	string join = u.getID() + " JOIN " + chan + "\n";
	string mode = ":127.0.0.1 MODE " + u.getNick() + " " + chan + " +nt\r\n";
	string listbegin = ":127.0.0.1 353 " + u.getNick() + " = " + chan + " :@" + u.getNick() + "\r\n";
	string listend = ":127.0.0.1 366 " + u.getNick() + " " + chan + " :End of /NAMES list.\r\n";

	cout << join << endl << mode << endl << listbegin << endl << listend << endl;

	send(u.getFd(), join.c_str(), join.length(), 0);
	send(u.getFd(), mode.c_str(), mode.length(), 0);
	send(u.getFd(), listbegin.c_str(), listbegin.length(), 0);
	send(u.getFd(), listend.c_str(), listend.length(), 0);
}

void Server::sendToChannel(string chan, string message)
{
	for (std::map<string, User>::iterator it = _channels[chan].getUsers().begin(); it != _channels[chan].getUsers().end(); it++)
    {
    	send(it->second.getFd(), message.c_str(), message.length(), 0);
    }
}

void Server::sendToChannelExcept(string chan, string message, User &except)
{
	for (std::map<string, User>::iterator it = _channels[chan].getUsers().begin(); it != _channels[chan].getUsers().end(); it++)
    {
		if (it->second.getNick() != except.getNick())
    		send(it->second.getFd(), message.c_str(), message.length(), 0);
    }
}

User *Server::getUser(int fd)
{
	for (unsigned int i = 0; i < _users.size(); i++)
	{
		if (fd == _users[i].getFd())
			return &_users[i];
	}
	return (NULL);
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
	string msg001 = ":127.0.0.1 001 " + u->getNick() + " :Welcome to the Internet Relay Network\r\n";
	string msg002 = ":127.0.0.1 002 " + u->getNick() + " :Your host is ft_irc, running version 0.1\r\n";
	string msg003 = ":127.0.0.1 003 " + u->getNick() + " :This server was created NOW\r\n";
	string msg004 = ":127.0.0.1 004 " + u->getNick() + " :ft_irc 0.1 * +itkol\r\n";

	send(u->getFd(), msg001.c_str(), msg001.length(), 0);
	send(u->getFd(), msg002.c_str(), msg002.length(), 0);
	send(u->getFd(), msg003.c_str(), msg003.length(), 0);
	send(u->getFd(), msg004.c_str(), msg004.length(), 0);
}

void Server::setPassword(string newPassword)
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

