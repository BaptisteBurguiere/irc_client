#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

void display_help(void)
{
	std::cout << "better_client usage:" << std::endl;
	std::cout << "./better_client <server_ip> <server_port>" << std::endl;
}

// bool try_connect

int main(int argc, char **argv)
{
	if (argc != 3)
		display_help();
	else
	{
		int socket_client = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in client_addr;
		client_addr.sin_addr.s_addr = inet_addr(argv[1]);
		client_addr.sin_family = AF_INET;
		client_addr.sin_port = htons(atoi(argv[2]));
		if (connect(socket_client, (const struct sockaddr *)&client_addr, sizeof(client_addr)) == -1)
		{
			std::cout << "Error: cannot connect to server " << argv[1] << " on port " << argv[2] << "." << std::endl;
			return 0;
		}
	}

	return 0;
}