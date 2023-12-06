#ifndef __SERVEUR_H__
#define __SERVEUR_H__

#include "Libs.hpp"
#include "User.hpp"
#include "Channel.hpp"


class Channel;

class Server
{
private:
	std::vector<struct pollfd>		fds;
	std::vector<User>				_users;
	std::map<string, Channel>	_channels;
	string						_password;
	addrinfo						*_servinfo;

	bool check_password(char *buf);

public:
	Server();
	Server(string password);
	~Server();
	
	void init();
	void init_clients();
	void handle_client(int new_fd);

	void leaveChannel(User &u, string msg);
	void joinExistingChannel(User &u, Channel &chan);
	void joinChannel(User &u, string msg);
	void connectClient(User *u);

	void new_client();
	void new_client(int fd);
	void disconnect_user(int client_i);
	void parse_user_info(int client_i, string parseUserInfo);

	//utils
	void createChannelMsg(User &u, string chan) const;
	void sendToChannel(string chan, string message);
	void sendToChannelExcept(string chan, string message, User &except);

	User	*getUser(int fd);

	void 		setPassword(string newPassword);
	const char	*getPassword()	const;
	int			getSocket()		const;


};

#endif
