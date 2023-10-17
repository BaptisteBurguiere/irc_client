#ifndef VIEW_H
# define VIEW_H

# include <ncurses.h>
# include <Window.h>
# include <mutex>

# define CHAT_WINDOW_SIZE 4
# define MESSAGE_COLOR 1
# define SERVER_COLOR 2
# define DM_COLOR 3
# define TAB_SIZE 4

# define TYPE_NO_MESSAGE -2
# define NO_TYPE -1
# define TYPE_MESSAGE 0
# define TYPE_SERVER_MESSAGE 1
# define TYPE_DM_MESSAGE 2

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
		bool isChatNewLine(void);

		void resize(void);

		void mutexLock(void);
		void mutexUnlock(void);

		bool writeInChat(std::string message, int type);
		void printChar(char c);

		void refresht(void);

		int getChatHeight(void);
		int getChatWidth(void);

	private:
		Window _chat_window;
		Window _input_window;
		mutable std::mutex _mutex;
};

#endif