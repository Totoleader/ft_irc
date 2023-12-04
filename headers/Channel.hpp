/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macote <macote@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 15:42:50 by macote            #+#    #+#             */
/*   Updated: 2023/12/04 15:22:40 by macote           ###   ########.fr       */
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

	std::vector<std::string> 		_moderator_name;

	std::string 					_password;
	// int								_userLimit;

	std::map<std::string, User>		_channelUsers;

	//mode
	// bool							_isInviteOnly;
	std::vector<std::string>		_inviteList;			

	// bool							_restrictTopic;

public:
	Channel();
	~Channel();

	//setPassword(...);
	//setOperator(...);
	//setUserLimit(...);
};


#endif // __CHANNEL_H__