#include "Libs.hpp"
#include "Server.hpp"
#include "Messages.hpp"

int main(int argc, char **argv)
{
	Messages message;

	if (argc != 3 )
	{
		cout << message.msgError("ERR_IRC_PARAM") << endl;	
		return (1);
	}

	Server serveur(argv[1], argv[2]);


	serveur.init();
	serveur.init_clients();

	return (0);
}
