#ifndef __SERVEUR_H__
#define __SERVEUR_H__

#include "Libs.hpp"
#include "Irc.hpp"
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
	void joinChannel(User &u, std::pair<string, string> chan);
	void connectClient(User *u);
	void sendMessage(User &u , string message);

	void new_client();
	void new_client(int fd);
	void disconnect_user(int client_i);
	void parse_user_info(int client_i, string parseUserInfo);

	//utils
	void createChannelMsg(User &u, string chan) const;
	void getAndJoinChannels(User &u, string channels);

	User	*getUser(int fd);

	void 		setPassword(string newPassword);
	const char	*getPassword()	const;
	int			getSocket()		const;

		//AJOUT DE ALEX // 
	void kickChannels(User &u, string str);
	void skipWhitespace(std::string &str);
	bool isUser(string user_to_kick, string channel_name);
	bool userExist(string user_to_invite);
	bool channelExist(string channel_name);
	void inviteChannels(User &u, string str);



};

#endif
