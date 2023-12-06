#include "headers/Libs.hpp"
#include "headers/Server.hpp"
//
int main()
{
	Server serveur("123");

	serveur.init();
	serveur.init_clients();

	return (0);
}
