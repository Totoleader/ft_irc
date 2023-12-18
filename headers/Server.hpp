#ifndef __SERVEUR_H__
#define __SERVEUR_H__

#include "Libs.hpp"
#include "Irc.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Messages.hpp"


class Channel;
class Messages;

class Server
{
private:
	std::vector<struct pollfd>		fds;
	std::vector<User>				_users;
	std::map<string, Channel>		_channels;
	std::string						_port;
	std::string						_password;
	std::string						_host;
	addrinfo						*_servinfo;

	bool check_password(char *buf);

public:
	Server();
	Server(string port, string password);
	~Server();
	
	void init();
	void init_clients();
	void handle_client(int new_fd);

	void leaveChannel(User &u, string msg);
	void joinExistingChannel(User &u, Channel &chan);
	void joinChannel(User &u, std::pair<string, string> chan);
	void connectClient(User *u);
	void sendMessage(User &u , string message);
	void changeMode(User &u , string message);

	void new_client();
	void new_client(int fd);
	void disconnect_user(int client_i);
	void parse_user_info(int client_i, string parseUserInfo);

	//utils
	void createChannelMsg(User &u, string chan) const;
	void getAndJoinChannels(User &u, string channels);

	User	*getUser(int fd);
	User	*getUser(string nick);

	void 		setPassword(string newPassword);
	const char	*getPassword()	const;
	int			getSocket()		const;

		//AJOUT DE ALEX // 
	const char	*getPortNumber()	const;
	string	getPort(); 
	string	getHost(); 
	void kickChannels(User &u, string str);
	void skipWhitespace(string &str);
	bool isUser(string user_to_kick, string channel_name);
	bool userExist(string user_to_invite);
	bool channelExist(string channel_name);
	void inviteChannels(User &u, string str);
	void setTopic(User &u, string str);
	void showTopic(User &u, string str);
	string printMsgError(string str);
};

#endif
