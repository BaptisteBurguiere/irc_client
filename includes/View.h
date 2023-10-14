#ifndef VIEW_H
# define VIEW_H

# include <ncurses.h>
# include <Window.h>
# include <mutex>

# define CHAT_WINDOW_SIZE 4
# define MESSAGE_COLOR 1
# define SERVER_COLOR 2
# define DM_COLOR 3

class View
{
	public:
		View(void);
		~View(void);

		View(const View &other);
		View &operator=(const View &other);

		void init(void);
		void destroy(void);

		void displayChat(void);
		void displayInput(void);

		void updateChatTitle(std::string channel_name, std::string topic);

		void setCursorInput(void);
		bool increaseCursorInput(void);
		bool decreaseCursorInput(void);

		void setCursorChat(void);
		bool increaseCursorChat(void);
		bool chatNewLine(void);

		void resize(void);

		void mutexLock(void);
		void mutexUnlock(void);

		void writeInChat(std::string message, int color);
		void printChar(char c);

	private:
		Window _chat_window;
		Window _input_window;
		mutable std::mutex _mutex;
};

#endif