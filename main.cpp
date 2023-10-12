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

// void threadInput(t_thread_vars *vars)
// {
// 	std::string msg = "";
// 	vars->running_mutex.lock();
// 	while (vars->is_running)
// 	{
// 		vars->running_mutex.unlock();
// 		int c = getch();
// 		switch (c)
// 		{
// 			case KEY_ENTER:
// 				std::cin << msg;
// 				msg = "";
// 				break;

// 			case KEY_BACKSPACE:
// 				if (msg.length() > 0)
// 				{
// 					msg.erase(msg.length() - 1);
// 				}
// 				break;

// 			default:
// 				msg += (char)c;
// 				printw("%c", c);
// 				break;
// 		}
// 		vars->running_mutex.lock();
// 	}
// }

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
		// initscr();
		listen(socket_client, 1);

		struct pollfd *pollfds = new struct pollfd[2];

		pollfds[0].fd = socket_client;
		pollfds[0].events = POLLIN;
		pollfds[0].revents = 0;

		pollfds[1].fd = 0;
		pollfds[1].events = POLLIN;
		pollfds[1].revents = 0;

		char msg[160];
		memset(msg, 0, 160);
		while (true)
		{
			if (poll(pollfds, 2, -1) == -1)
				perror("poll");
			for (int i = 0; i < 2; ++i)
			{
				if((pollfds[i].revents & POLLIN) == POLLIN)
				{
					memset(msg, 0, 160);
					if (i == 0)
					{
						int size = recv(pollfds[i].fd, &msg, sizeof(msg), 0);
						if (size == -1)
							perror("recv");
						if (size == 0)
						{
							std::cout << COLOR_SERVER << "You have been kicked of the server." << COLOR_RESET << std::endl;
							close(socket_client);
							// endwin();
							return 0;
						}
						else
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
					else
					{
						std::string tmp;
						getline(std::cin, tmp);
						if (tmp == COMMAND_EXIT)
						{
							close(socket_client);
							// endwin();
							return 0;
						}

						int i;
						for(i = 0; tmp[i] && i < 158; i++)
							msg[i] = tmp[i];
						msg[i++] = '\n';
						msg[i] = '\0';

						if (send(pollfds[0].fd, &msg, strlen(msg), 0) == -1)
							perror("send");
					}
				}
			}
		}
	}
	// endwin();
	return 0;
}