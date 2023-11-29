#include <string>
#include <netdb.h>

class User
{
private:
	std::string				_nick;
	std::string				_login_name;
	std::string				_real_name;
	bool					op;
	int						fd;
	struct sockaddr_storage	*sock;
	
public:
	User();
	~User();

	bool	checkNameValidity(std::string const & name);
};