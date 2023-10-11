#include <irc_client.h>

void displayHelp(void)
{
	std::cout << "better_client usage:" << std::endl;
	std::cout << "./better_client <server_ip> <server_port>" << std::endl;
}

t_message parseMessage(char *msg)
{
	t_message parsed_message;
	std::string str_msg(msg);
	str_msg.erase(str_msg.length() - 1);

	if (str_msg.find(SERVER_HEADER) != std::string::npos)
	{
		int start_header = str_msg.find(SERVER_HEADER);
		int end_header = str_msg.find(SERVER_HEADER, start_header + 1);
		int header_length = strlen(SERVER_HEADER);

		std::string str_type = str_msg.substr(start_header + header_length, end_header - (start_header + header_length));
		parsed_message.type = atoi(str_type.c_str());
		parsed_message.message = str_msg.substr(end_header + header_length);
	}
	else
	{
		parsed_message.type = -1;
		parsed_message.message = str_msg;
	}
	return parsed_message;
}

int main(int argc, char **argv)
{
	if (argc != 3)
		displayHelp();
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
		char msg[1024];
		while (true)
		{
			memset(msg, 0, strlen(msg));
			if (recv(socket_client, &msg, 1024, 0) > 0)
			{
				t_message parsed_message = parseMessage(msg);

				switch (parsed_message.type)
				{
					case NO_TYPE: case TYPE_MESSAGE:
						std::cout << parsed_message.message << std::endl;
						break;

					case TYPE_SERVER_MESSAGE:
						std::cout << COLOR_SERVER << "SERVER: " << parsed_message.message << COLOR_RESET << std::endl;
						break;

					default:
						break;
				}
			}
		}
		
	}
	return 0;
}