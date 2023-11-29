#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <iostream>
#include <future>
#include <thread>

int main()
{
	int sock;// fd du socket
	addrinfo *servinfo;//struct contenant info importantes pour la suite
	addrinfo hints;//genre de template pour seter manulement des trucs dans servinfo

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // seuelement IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE; //fill in l'ip par défaut

	getaddrinfo(NULL, "6667", &hints, &servinfo);//fill in servinfo struct

	sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);//initialise le socket

	if (bind(sock, servinfo->ai_addr, servinfo->ai_addrlen) != 0)//initialise le socket part 2
	{
		std::cerr << "Bind failed." << std::endl;
		return (1);
	}

	listen(sock, 10);//sois prêt a accepter des clients

	{//nouveau client
	struct sockaddr_storage client_info;
	socklen_t	addr_size;
	int new_fd;

	addr_size = sizeof(client_info);
	}

	//accepte la connection
	new_fd = accept(sock, (struct sockaddr *)&client_info, &addr_size);

	char buf[100];
	while (1)
	{
		memset(buf, 0, 100);
		recv(new_fd, buf, 100, 0);//lis le message et le met dans le buffer

		send(new_fd, buf, strlen(buf), 0);//renvoie au client ce qui est dans le buffer
		// std::cout << buf << std::endl;
	}

	freeaddrinfo(servinfo);//free
	
	return (0);
}