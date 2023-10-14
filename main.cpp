// #include <irc_client.h>

// void displayHelp(void)
// {
// 	std::cout << "better_client usage:" << std::endl;
// 	std::cout << "./better_client <server_ip> <server_port>" << std::endl;
// }

// t_message parseMessage(char *msg)
// {
// 	t_message parsed_message;
// 	std::string str_msg(msg);
// 	str_msg.erase(str_msg.length() - 1);

// 	if (str_msg.find(SERVER_HEADER) != std::string::npos)
// 	{
// 		int start_header = str_msg.find(SERVER_HEADER);
// 		int end_header = str_msg.find(SERVER_HEADER, start_header + 1);
// 		int header_length = strlen(SERVER_HEADER);

// 		std::string str_type = str_msg.substr(start_header + header_length, end_header - (start_header + header_length));
// 		parsed_message.type = atoi(str_type.c_str());
// 		parsed_message.message = str_msg.substr(end_header + header_length);
// 	}
// 	else
// 	{
// 		parsed_message.type = -1;
// 		parsed_message.message = str_msg;
// 	}
// 	return parsed_message;
// }

// void threadInput(t_thread_vars *vars)
// {
// 	std::string msg_str = "";
// 	vars->running_mutex.lock();
// 	while (vars->is_running)
// 	{
// 		vars->running_mutex.unlock();
// 		int c = getch();
// 		switch (c)
// 		{
// 			case 10:
// 				if (msg_str.length() > 0)
// 				{
// 					if (msg_str == COMMAND_EXIT)
// 					{
// 						vars->running_mutex.lock();
// 						vars->is_running = false;
// 						vars->running_mutex.unlock();
// 						return;
// 					}
// 					msg_str += '\n';
// 					if (send(vars->fd.fd, msg_str.c_str(), msg_str.length(), 0) == -1)
// 						perror("send");
// 					msg_str = "";
// 				}
// 				break;

// 			case 127:
// 				if (msg_str.length() > 0)
// 				{
// 					msg_str.erase(msg_str.length() - 1);
// 				}
// 				break;

// 			default:
// 				msg_str += (char)c;
// 				// printw("%c", c);
// 				break;
// 		}
// 		vars->running_mutex.lock();
// 	}
// }

// int main(int argc, char **argv)
// {
// 	if (argc != 3)
// 		displayHelp();
// 	else
// 	{
// 		t_thread_vars thread_vars;
// 		thread_vars.is_running = true;
// 		int socket_client = socket(AF_INET, SOCK_STREAM, 0);
// 		struct sockaddr_in client_addr;
// 		client_addr.sin_addr.s_addr = inet_addr(argv[1]);
// 		client_addr.sin_family = AF_INET;
// 		client_addr.sin_port = htons(atoi(argv[2]));
// 		if (connect(socket_client, (const struct sockaddr *)&client_addr, sizeof(client_addr)) == -1)
// 		{
// 			std::cout << "Error: cannot connect to server " << argv[1] << " on port " << argv[2] << "." << std::endl;
// 			return 0;
// 		}
// 		initscr();
// 		listen(socket_client, 1);

// 		thread_vars.fd.fd = socket_client;
// 		thread_vars.fd.events = POLLIN;
// 		thread_vars.fd.revents = 0;

// 		char msg[160];
// 		memset(msg, 0, 160);

// 		std::thread inputThread(threadInput, &thread_vars);
// 		thread_vars.running_mutex.lock();
// 		while (thread_vars.is_running)
// 		{
// 			thread_vars.running_mutex.unlock();
// 			if (poll(&thread_vars.fd, 1, -1) == -1)
// 				perror("poll");
// 			if((thread_vars.fd.revents & POLLIN) == POLLIN)
// 			{
// 				memset(msg, 0, 160);

// 				int size = recv(thread_vars.fd.fd, &msg, sizeof(msg), 0);
// 				if (size == -1)
// 					perror("recv");
// 				if (size == 0)
// 				{
// 					std::cout << COLOR_SERVER << "You have been kicked of the server." << COLOR_RESET << std::endl;
// 					close(socket_client);
// 					endwin();
// 					return 0;
// 				}
// 				else
// 				{
// 					t_message parsed_message = parseMessage(msg);
// 					switch (parsed_message.type)
// 					{
// 						case NO_TYPE: case TYPE_MESSAGE:
// 							// std::cout << parsed_message.message << std::endl;
// 							printw("%s\n", parsed_message.message.c_str());
// 							refresh();
// 							break;

// 						case TYPE_SERVER_MESSAGE:
// 							std::cout << COLOR_SERVER << "SERVER: " << parsed_message.message << COLOR_RESET << std::endl;
// 							break;

// 						default:
// 							break;
// 					}
// 				}
// 				// else
// 				// {
// 				// 	std::string tmp;
// 				// 	getline(std::cin, tmp);
// 				// 	if (tmp == COMMAND_EXIT)
// 				// 	{
// 				// 		close(socket_client);
// 				// 		endwin();
// 				// 		return 0;
// 				// 	}

// 				// 	int i;
// 				// 	for(i = 0; tmp[i] && i < 158; i++)
// 				// 		msg[i] = tmp[i];
// 				// 	msg[i++] = '\n';
// 				// 	msg[i] = '\0';

// 				// 	if (send(pollfds[0].fd, &msg, strlen(msg), 0) == -1)
// 				// 		perror("send");
// 				// }
// 			}
// 			thread_vars.running_mutex.lock();
// 		}
// 	}
// 	endwin();
// 	return 0;
// }

// #include <View.h>
// #include <irc_client.h>

// int main(void)
// {
// 	View view;
// 	view.init();
// 	view.updateChannelName("Better Client");

// 	std::string input = "";
// 	while (true)
// 	{
// 		view.setCursorInput();
// 		int c = getch();
// 		switch (c)
// 		{
// 			case ENTER:
// 				if (input == COMMAND_EXIT)
// 				{
// 					view.destroy();
// 					return 0;
// 				}
// 				if (input.length() > 0)
// 				{
// 					for (int i = 0; i < (int)input.length(); ++i)
// 					{
// 						view.setCursorChat();
// 						if (view.increaseCursorChat())
// 							printw("%c", input[i]);
// 						else
// 							break;
// 					}
// 					view.chatNewLine();
// 					input = "";
// 					view.displayInput();
// 				}
// 				break;

// 			case BACKSPACE:
// 				if (view.decreaseCursorInput())
// 				{
// 					input.erase(input.length() - 1);
// 					view.setCursorInput();
// 					printw(" ");
// 				}
// 				break;

// 			case RESIZE:
// 				view.resize();
// 				view.displayInput();
// 				view.displayChat();
// 				for (int i = 0; i < (int)input.length(); ++i)
// 				{
// 					view.setCursorInput();
// 					if (view.increaseCursorInput())
// 						printw("%c", input[i]);
// 					else
// 					{
// 						input.erase(i);
// 						break;
// 					}
// 				}
// 				break;

// 			case ESCAPE:
// 				view.destroy();
// 				return 0;
// 				break;

// 			default:
// 				if (view.increaseCursorInput())
// 				{
// 					printw("%c", c);
// 					input += char(c);
// 				}
// 				break;
// 		}
// 	}


// 	view.destroy();
// 	return 0;
// }

#include <Model.h>
#include <View.h> 
#include <Controller.h>

void displayHelp(void)
{
	std::cout << "better_client usage:" << std::endl;
	std::cout << "./better_client <server_ip> <server_port>" << std::endl;
}

int main(int argc, char **argv)
{
	if (argc != 3)
		displayHelp();
	else
	{
		View view;
		Model model;
		Controller controller(model, view);
		if (!controller.init(argv[1], argv[2]))
			std::cout << "Error: cannot connect to server " << argv[1] << " on port " << argv[2] << "." << std::endl;
		else
		{
			controller.mainLoop();
			controller.destroy();
		}
	}
	return 0;
}