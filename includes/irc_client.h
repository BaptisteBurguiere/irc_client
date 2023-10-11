#ifndef IRC_CLIENT_H
# define IRC_CLIENT_H

# include <sys/types.h>
# include <sys/socket.h>
# include <iostream>
# include <string.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <unistd.h>
# include <cstring>

# define SERVER_HEADER "__HEADER__"
# define NO_TYPE -1
# define TYPE_MESSAGE 0
# define TYPE_SERVER_MESSAGE 1
# define TYPE_DISCONNECT 2
# define TYPE_USERNAME 3
# define TYPE_NEW_USER 4
# define TYPE_INCORRECT_PASSWORD 5

# define COMMAND_EXIT "EXIT"

# define COLOR_RESET "\033[0m"
# define COLOR_SERVER "\033[31m"

typedef struct s_message
{
	int type;
	std::string message;
} t_message;



#endif