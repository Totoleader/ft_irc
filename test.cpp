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
#include <unistd.h>

void check_password(char *buf, const char *password, int *password_ok, int new_fd)
{
	if (*password == '\0')
	{
		*password_ok = true;
	}
	else if (!strncmp(buf, "PASS", 4))
	{
		std::cout << std::endl << "client send:" << buf << std::endl;
		if (!strncmp(buf + 5, password, strlen(password)) && *(buf + 5 + strlen(password) + 1) != '\r')
		{
			*password_ok = 1;
			std::cout << "right password" << std::endl;
		}
		else
		{
			std::cout << "wrong password" << std::endl;
			close(new_fd);
		}
		send(new_fd, buf, strlen(buf), 0);//renvoie au client ce qui est dans le buffer
	}
}

void *handle_client(int new_fd, const char *password)
{
	char buf[100];
	int password_is_ok = 0;

	memset(buf, 0, 100);
	recv(new_fd, buf, 100, 0);//lis le message et le met dans le buffer
	check_password(buf, password, &password_is_ok, new_fd);
	if (password_is_ok)
	{
		char msg1[100] = ":127.0.0.1 001 Sam :Welcome to the Internet Relay Network\n";
		int len1 = strlen(msg1);
		send(new_fd, msg1, len1, 0);

		char msg2[100] = ":127.0.0.1 002 Sam :Your host is 127.0.0.1 running version 0.1\n";
		int len2 = strlen(msg2);
		send(new_fd, msg2, len2, 0);

		char msg3[100] = ":127.0.0.1 003 Sam :This server was created 29-11-2023\n";
		int len3 = strlen(msg3);
		send(new_fd, msg3, len3, 0);

		char msg4[100] = ":127.0.0.1 004 Sam :ft_irc 0.1 i i\n";
		int len4 = strlen(msg4);
		send(new_fd, msg4, len4, 0);
	}
	while (password_is_ok)
	{
		memset(buf, 0, 100);
		if (recv(new_fd, buf, 100, 0) < 1)
			break;
		std::cout << std::endl << "client send:" << buf << std::endl;
		send(new_fd, buf, strlen(buf), 0);//renvoie au client ce qui est dans le buffer
	}
	return (NULL);
}

void init_server(addrinfo *servinfo, int *sock)
{
	addrinfo hints;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, "6667", &hints, &servinfo);

	*sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	if (bind(*sock, servinfo->ai_addr, servinfo->ai_addrlen) != 0)
	{
		std::cerr << "Bind failed." << std::endl;
		return ;
	}

	listen(*sock, 10);
}

void init_clients(int sock)
{
	int new_fd;
	struct sockaddr_storage client_info;
	const char password[] = "123";
	socklen_t	addr_size = sizeof(client_info);

	while (1)
	{
		new_fd = accept(sock, (struct sockaddr *)&client_info, &addr_size);
		
		handle_client(new_fd, password);//launché dans un thread
	}
}

int main()
{
	addrinfo *servinfo;
	int sock;

	init_server(servinfo, &sock);

	init_clients(sock);


	freeaddrinfo(servinfo);
	return (0);
}