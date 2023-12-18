#ifndef __MESSAGES_H_
# define __MESSAGES_H_

# include <string>
# include <iostream>
# include <netdb.h>
# include <arpa/inet.h>
# include <vector>
# include <Libs.hpp>
#include "Server.hpp"

class Messages
{
private:

public:
	Messages();
	~Messages();
    string  msgError(string Error);
    string  msgValide(string Valid);

};

#endif