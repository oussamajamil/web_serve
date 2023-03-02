#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include "./request.hpp"

#define BUFF_SIZE 1048576

int main()
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		std::cout << "Error creating socket" << std::endl;
		return 1;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4000);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		std::cout << "Error binding socket" << std::endl;
		return 1;
	}

	if (listen(server_fd, 10) == -1)
	{
		std::cout << "Error listening on socket" << std::endl;
		return 1;
	}

	while (true)
	{
		int client_fd = accept(server_fd, NULL, NULL);
		if (client_fd == -1)
		{
			std::cout << "Error accepting connection" << std::endl;
			return 1;
		}

		char buffer[BUFF_SIZE];

		int read_size = recv(client_fd, buffer, BUFF_SIZE, 0);
		if (read_size == -1)
		{
			std::cout << "Error reading from socket" << std::endl;
			return 1;
		}
		std::string request(buffer);

		Request req(request);
		close(client_fd);
	}
}