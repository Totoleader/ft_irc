/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macote <macote@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 15:42:50 by macote            #+#    #+#             */
/*   Updated: 2023/12/05 15:11:23 by macote           ###   ########.fr       */
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
	std::string 					_name;
	std::string 					_topic;

	std::vector<std::string> 		_moderatorName;

	std::string 					_password;
	// int								_userLimit;

	std::map<std::string, User>		_channelUsers;

	//mode
	// bool							_isInviteOnly;
	std::vector<std::string>		_inviteList;

	// bool							_restrictTopic;

public:
	Channel();
	Channel(std::string name, User &u);
	~Channel();

	void addUser(User &user);
	void addModerator(std::string modName);
	std::map <std::string, User> &getUsers();
	std::vector<std::string> &getmoderatorName();
	//setPassword(...);
	//setOperator(...);
	//setUserLimit(...);
	std::string	getName();
};


#endif // __CHANNEL_H__