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
# define ARROW_UP 279165
# define ARROW_DOWN 279166

# define SERVER_HEADER "__HEADER__"
# define TYPE_NO_MESSAGE -2
# define NO_TYPE -1
# define TYPE_MESSAGE 0
# define TYPE_SERVER_MESSAGE 1
# define TYPE_DM_MESSAGE 2
# define TYPE_CHANNEL_MESSAGE 3
# define TYPE_TOPIC_MESSAGE 4

# define COMMAND_EXIT "EXIT"

# define MSG_SIZE 1024

class Controller
{
	public:
		Controller(Model model, View view);
		~Controller(void);

		typedef struct s_thread_vars
		{
			int socket_client;
			bool *is_running;
			std::mutex *running_mutex;
			Model *model;
			View *view;
		} t_thread_vars;

		bool init(std::string ip, std::string port);
		void destroy(void);

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
		void updateChat(bool mode);
};

#endif