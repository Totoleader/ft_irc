#include "Libs.hpp"
#include "Irc.hpp"

bool checkInvited(User &u, Channel &chan)
{
	if (!chan.isInviteOnly() || chan.isWhitelisted(u)) //bouncer
		return true;
	return false;
}

bool passIsOk(Channel &chan, string password)
{
	if (chan.getPassword().empty()) //no password -> OK
		return true;
	else if (chan.getPassword() == password) // good pass -> OK
		return true;
	return false; // Wrong pass
}
