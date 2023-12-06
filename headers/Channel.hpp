/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macote <macote@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 15:42:50 by macote            #+#    #+#             */
/*   Updated: 2023/12/06 15:55:13 by macote           ###   ########.fr       */
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
	std::vector<string>		_inviteList;

	// bool							_restrictTopic;

public:
	Channel();
	Channel(string name, User &u);
	~Channel();

	void addUser(User &user);
	void addModerator(string modName);
	std::map <string, User> &getUsers();
	std::vector<string> &getmoderatorName();
	//setPassword(...);
	//setOperator(...);
	//setUserLimit(...);
	string	getName();
	bool isOperator(User &u);
	bool isInviteOnly();
	bool isWhitelisted(User &u);
};


#endif // __CHANNEL_H__