#ifndef CONTROLLER_H
# define CONTROLLER_H

# include <View.h>
# include <Model.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <iostream>
# include <string.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <unistd.h>
# include <cstring>
# include <thread>
# include <mutex>
# include <poll.h>

# define RESIZE 410
# define ENTER 10
# define BACKSPACE 127
# define ESCAPE 27

# define SERVER_HEADER "__HEADER__"
# define NO_TYPE -1
# define TYPE_MESSAGE 0
# define TYPE_SERVER_MESSAGE 1
# define TYPE_DM_MESSAGE 2
// # define TYPE_DISCONNECT 2
// # define TYPE_USERNAME 3
// # define TYPE_NEW_USER 4
// # define TYPE_INCORRECT_PASSWORD 5

# define COMMAND_EXIT "EXIT"

# define MSG_SIZE 1024

class Controller
{
	public:
		Controller(Model model, View view);
		~Controller(void);

		void destroy(void);

		typedef struct s_thread_vars
		{
			int socket_client;
			bool *is_running;
			std::mutex *running_mutex;
			Model *model;
			View *view;
		} t_thread_vars;

		bool init(std::string ip, std::string port);
		void mainLoop(void);


	private:
		Model _model;
		View _view;
		bool _is_running;
		std::mutex _running_mutex;
		t_thread_vars thread_vars;
		std::thread _input_thread;
		struct pollfd _pollfd;

		static void inputThread(t_thread_vars *vars);
		t_message parseMessage(char *msg);
};

#endif