#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include "./request.hpp"

int main()
{

	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	if (socketfd == -1)
	{
		std::cout << "Error creating socket" << std::endl;
		return -1;
	}
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		std::cout << "Error binding socket" << std::endl;
		return -1;
	}

	while (true)
	{
		if (listen(socketfd, 10) == -1)
		{
			std::cout << "Error listening on socket" << std::endl;
			return -1;
		}

		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int client_socket = accept(socketfd, (struct sockaddr *)&client_addr, &client_addr_len);

		if (client_socket == -1)
		{
			std::cout << "Error accepting connection" << std::endl;
			return -1;
		}

		char buffer[1024];
		int bytes_read = recv(client_socket, buffer, 1024, 0);
		if (bytes_read == -1)
		{
			std::cout << "Error reading from socket" << std::endl;
			return -1;
		}
		Request *req =new Request(buffer);
		close(client_socket);
	}
	return 0;
}