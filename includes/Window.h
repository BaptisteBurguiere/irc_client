#ifndef WINDOW_H
# define WINDOW_H

# include <ncurses.h>
# include <string>

class Window
{
	public:
		Window(void);
		Window(int height, int width, int start_y, int start_x, std::string title = "");
		~Window(void);

		Window(const Window &other);
		Window &operator=(const Window &other);

		void display(void);
		void refreshWindow(void);

		bool increaseCursor(void);
		bool decreaseCursor(void);

		std::string getTitle(void);
		void setTitle(std::string new_title);

		int getHeight(void);
		void setHeight(int new_height);

		int getStartX(void);
		void setStartX(int new_start_x);

		int getStartY(void);
		void setStartY(int new_start_y);

		int getWidth(void);
		void setWidth(int new_width);

		int getCursorX(void);
		void setCursorX(int new_cursor_x);

		int getCursorY(void);
		void setCursorY(int new_cursor_y);

	private:
		WINDOW *_window;

		std::string _title;
		int _height;
		int _width;

		int _start_x;
		int _start_y;

		int _cursor_x;
		int _cursor_y;
};

#endif