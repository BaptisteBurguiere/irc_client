#ifndef VIEW_H
# define VIEW_H

# include <ncurses.h>
# include <Window.h>

# define CHAT_WINDOW_SIZE 4
# define RESIZE 410
# define ENTER 10
# define BACKSPACE 127

class View
{
	public:
		View(void);
		~View(void);

		void init(void);
		void destroy(void);

		void displayChat(void);
		void displayInput(void);

		void updateChannelName(std::string name);

		void setCursorInput(void);
		bool increaseCursorInput(void);
		bool decreaseCursorInput(void);

		void setCursorChat(void);
		bool increaseCursorChat(void);
		bool chatNewLine(void);

		void resize(void);

	private:
		Window _chat_window;
		Window _input_window;
		// std::vector<std::string> _chat_history;
};

#endif