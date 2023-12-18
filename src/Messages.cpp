#include "Libs.hpp"
#include "Server.hpp"
#include "Messages.hpp"

Messages::Messages()
{
}

Messages::~Messages()
{
}

string  Messages::msgError(string Error)
{
    if (Error.substr(0, 13) == "ERR_IRC_PARAM")
        return ("Error: ./ircserv <port> <password>");
    else if (Error.substr(0, 15) == "ERR_BIND_FAILED")
        return ("Bind failed");
    else if (Error.substr(0, 3) == "461")
        return(":" + getHost() + "461 KICK :Not enough parameters\r\n");
}

string  Messages::msgValide(string Valid)
{
    return(Valid);
}