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