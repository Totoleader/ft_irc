/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macote <macote@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 15:42:50 by macote            #+#    #+#             */
/*   Updated: 2023/12/13 14:47:24 by macote           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include "Libs.hpp"
#include "User.hpp"
#include "Server.hpp"

// class User;
class Channel
{
private:
	string 					_name;
	string 					_topic;

	std::vector<string> 		_moderatorName;

	string 					_password;
	// int								_userLimit;

	std::map<string, User>		_channelUsers;

	//mode
	bool							_isInviteOnly;
	std::vector<User>		_inviteList;
	// bool							_restrictTopic;

public:
	Channel();
	Channel(string name, User &u, string password);
	~Channel();

	void addUser(User &user);
	void addModerator(string modName);
	void giveOp(User &giver, User &reveiver);
	std::map <string, User> &getUsers();
	std::vector<string> &getmoderatorName();

	void sendToChannel(string message);
	void sendToChannelExcept(string message, User &except);
	//setPassword(...);
	//setOperator(...);
	//setUserLimit(...);
	string	getName();
	bool isOperator(User &u);
	bool isInviteOnly();
	void setInviteOnly(bool state);
	bool isWhitelisted(User &u);
	string getPassword();
	void	addToWhiteList(User &newUser);


};


#endif // __CHANNEL_H__