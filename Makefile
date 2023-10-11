NAME=better_client
CC=c++
FLAGS=-Wall -Wextra -Werror -g
SRCS=$(wildcard *.cpp) \

${NAME} :
	$(CC) $(FLAGS) -Iincludes $(SRCS) -o $(NAME)

all : ${NAME}

clean :
	rm -rf ${NAME}

fclean : clean

re : fclean all