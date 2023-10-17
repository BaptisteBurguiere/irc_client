#include <Controller.h>

Controller::Controller(Model model, View view) : _model(model), _view(view) {}

Controller::~Controller(void) {}

void Controller::destroy(void)
{
	this->_input_thread.join();
	close(this->_pollfd.fd);
	this->_view.destroy();
}

bool Controller::init(std::string ip, std::string port)
{
	int socket_client = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in client_addr;
	client_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(atoi(port.c_str()));
	if (connect(socket_client, (const struct sockaddr *)&client_addr, sizeof(client_addr)) == -1)
		return false;
	listen(socket_client, 1);
	this->_pollfd.fd = socket_client;
	this->_pollfd.events = POLLIN;
	this->_pollfd.revents = 0;


	this->_model.init();
	this->_view.init();
	this->_is_running = true;

	this->thread_vars.socket_client = socket_client;
	this->thread_vars.is_running = &this->_is_running;
	this->thread_vars.running_mutex = &this->_running_mutex;
	this->thread_vars.model = &this->_model;
	this->thread_vars.view = &this->_view;
	this->_input_thread = std::thread(Controller::inputThread, &this->thread_vars);
	return true;
}

t_message Controller::parseMessage(char *msg)
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

void Controller::updateChat(void)
{
	static int history = 0;

	this->_view.displayChat();
	int temp = history;
	t_message message = this->_model.getChatHistoryAt(temp);
	while (message.type != TYPE_NO_MESSAGE)
	{
		if (!this->_view.writeInChat(message.message, message.type))
		{
			history++;
			this->updateChat();
			return;
		}
		temp++;
		message = this->_model.getChatHistoryAt(temp);
	}
	history++;
}

void Controller::inputThread(t_thread_vars *vars)
{
	std::string input = "";
	// int history = 0;
	// std::string temp;
	vars->running_mutex->lock();
	while (*(vars->is_running))
	{
		vars->running_mutex->unlock();

		int c = getch();
		vars->running_mutex->lock();
		if (!(*(vars->is_running)))
		{
			vars->running_mutex->unlock();
			return;
		}
		vars->running_mutex->unlock();

		switch (c)
		{
			case ENTER:
				if (input == COMMAND_EXIT)
				{
					vars->running_mutex->lock();
					*(vars->is_running) = false;
					vars->running_mutex->unlock();
					return;
				}
				else if (input.length() > 0)
				{
					// history = 0;
					vars->model->addInputHistory(input);
					if (send(vars->socket_client, input.c_str(), input.length(), 0) == -1)
 						perror("send");
 					input = "";
 					vars->view->mutexLock();
 					vars->view->displayInput();
 					vars->view->setCursorInput();
 					vars->view->refresht();
 					vars->view->mutexUnlock();
				}
				break;

			case BACKSPACE:
				vars->view->mutexLock();
				if (vars->view->decreaseCursorInput())
				{
					input.erase(input.length() - 1);
					vars->view->setCursorInput();
					vars->view->printChar(' ');
					vars->view->setCursorInput();
				}
				vars->view->refresht();
				vars->view->mutexUnlock();
				break;

			// case ARROW_UP:
			// 	vars->model->mutexLock();
			// 	temp = vars->model->getInputHistoryAt(history);
			// 	vars->model->mutexUnlock();
			// 	if (temp != "")
			// 	{
			// 		input = temp;
			// 		history++;

			// 		vars->view->mutexLock();
			// 		vars->view->displayInput();
			// 		for (int i = 0; i < (int)input.length(); ++i)
			// 		{
			// 			vars->view->setCursorInput();
			// 			if (vars->view->increaseCursorInput())
			// 				vars->view->printChar(input[i]);
			// 			else
			// 			{
			// 				input.erase(i);
			// 				break;
			// 			}
			// 		}
			// 		vars->view->refresht();
			// 		vars->view->mutexUnlock();
			// 	}
			// 	break;

			// case ARROW_DOWN:
			// 	vars->model->mutexLock();
			// 	temp = temp = vars->model->getInputHistoryAt(history);
			// 	vars->model->mutexUnlock();
			// 	if (temp != "")
			// 	{
			// 		input = temp;
			// 		if (history > 0)
			// 			history--;

			// 		vars->view->mutexLock();
			// 		vars->view->displayInput();
			// 		for (int i = 0; i < (int)input.length(); ++i)
			// 		{
			// 			vars->view->setCursorInput();
			// 			if (vars->view->increaseCursorInput())
			// 				vars->view->printChar(input[i]);
			// 			else
			// 			{
			// 				input.erase(i);
			// 				break;
			// 			}
			// 		}
			// 		vars->view->refresht();
			// 		vars->view->mutexUnlock();
			// 	}
			// 	break;

			case ESCAPE:
				vars->running_mutex->lock();
				*(vars->is_running) = false;
				vars->running_mutex->unlock();
				return;
				break;

			case RESIZE:
				vars->view->mutexLock();
				vars->view->resize();
				vars->view->displayInput();
				vars->view->displayChat();
				for (int i = 0; i < (int)input.length(); ++i)
				{
					vars->view->setCursorInput();
					if (vars->view->increaseCursorInput())
						vars->view->printChar(input[i]);
					else
					{
						input.erase(i);
						break;
					}
				}
				vars->view->setCursorInput();
				vars->view->refresht();
				vars->view->mutexUnlock();
				break;

			default:
				vars->view->mutexLock();
				vars->view->setCursorInput();
				if (vars->view->increaseCursorInput())
				{
					vars->view->printChar(c);
					input += char(c);
				}
				vars->view->refresht();
				vars->view->mutexUnlock();
				break;
		}
		vars->running_mutex->lock();
	}
}

void Controller::mainLoop(void)
{
	char msg[MSG_SIZE];
	memset(msg, 0, MSG_SIZE);

	this->_running_mutex.lock();
	while (this->_is_running)
	{
		this->_running_mutex.unlock();

		poll(&this->_pollfd, 1, 1);
			// perror("poll");
		if((this->_pollfd.revents & POLLIN) == POLLIN)
		{
			memset(msg, 0, MSG_SIZE);
			int size = recv(this->_pollfd.fd, &msg, sizeof(msg), 0);
			if (size == -1)
				perror("recv");
			else if (size == 0)
			{
				t_message new_message;
				new_message.message = "You have been kicked of the server.";
				new_message.type = TYPE_SERVER_MESSAGE;
				this->_model.mutexLock();
				this->_model.addChatHistory(new_message);
				this->_view.mutexLock();
				if (!this->_view.writeInChat(new_message.message, new_message.type))
					this->updateChat();
				this->_view.refresht();
				this->_view.mutexUnlock();
				this->_model.mutexUnlock();
				this->_running_mutex.lock();
				this->_is_running = false;
				this->_running_mutex.unlock();
				return;
			}
			else
			{
				t_message parsed_message = parseMessage(msg);
				this->_model.mutexLock();
				this->_model.addChatHistory(parsed_message);
				this->_view.mutexLock();
				if (!this->_view.writeInChat(parsed_message.message, parsed_message.type))
					this->updateChat();
				this->_view.refresht();
				this->_view.mutexUnlock();
				this->_model.mutexUnlock();
			}
		}
		this->_running_mutex.lock();
	}
}