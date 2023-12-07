#ifndef __IRC_H__
#define __IRC_H__

#include "Channel.hpp"
#include "User.hpp"

class Channel;

std::map<std::string, std::string>	parseChannels(std::string channels);

bool checkInvited(User &u, Channel &chan);
bool passIsOk(Channel &chan, string password);

#endif