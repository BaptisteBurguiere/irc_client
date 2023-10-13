#include <Window.h>

Window::Window(void)
{
	this->_height = 0;
	this->_width = 0;
	this->_start_y = 0;
	this->_start_x = 0;
	this->_title = "";
	this->_cursor_y = 1;
	this->_cursor_x = 1;

	this->_window = newwin(this->_height, this->_width, this->_start_y, this->_start_x);

}

Window::Window(int height, int width, int start_y, int start_x, std::string title)
{
	this->_height = height;
	this->_width = width;
	this->_start_y = start_y;
	this->_start_x = start_x;
	this->_title = title;
	this->_cursor_y = 1;
	this->_cursor_x = 1;

	this->_window = newwin(this->_height, this->_width, this->_start_y, this->_start_x);
}

Window::~Window(void) {}

void Window::display(void)
{
	refresh();
	box(this->_window, 0, 0);
	if (this->_title.length() > 0)
		mvwprintw(this->_window, 0, 2, this->_title.c_str());
	wrefresh(this->_window);
	this->_cursor_y = 1;
	this->_cursor_x = 1;
}

void Window::refreshWindow(void)
{
	delwin(this->_window);
	this->_window = newwin(this->_height, this->_width, this->_start_y, this->_start_x);
}

bool Window::increaseCursor(void)
{
	if (this->_cursor_x >= this->_width - 2)
	{
		if (this->_cursor_y < this->_height - 2)
		{
			this->_cursor_y++;
			this->_cursor_x = 1;
		}
		else
			return false;
	}
	else
		this->_cursor_x++;
	return true;
}

bool Window::decreaseCursor(void)
{
	if (this->_cursor_x <= 1)
	{
		if (this->_cursor_y > 1)
		{
			this->_cursor_y--;
			this->_cursor_x = this->_width - 2;
		}
		else
			return false;
	}
	else
		this->_cursor_x--;
	return true;
}

std::string Window::getTitle(void) {return this->_title;}
void Window::setTitle(std::string new_title) {this->_title = new_title;}

int Window::getHeight(void) {return this->_height;}
void Window::setHeight(int new_height) {this->_height = new_height;}

int Window::getWidth(void) {return this->_width;}
void Window::setWidth(int new_width) {this->_width = new_width;}

int Window::getStartY(void) {return this->_start_y;}
void Window::setStartY(int new_start_y) {this->_start_y = new_start_y;}

int Window::getStartX(void) {return this->_start_x;}
void Window::setStartX(int new_start_x) {this->_start_x = new_start_x;}

int Window::getCursorY(void) {return this->_cursor_y;}
void Window::setCursorY(int new_cursor_y) {this->_cursor_y = new_cursor_y;}

int Window::getCursorX(void) {return this->_cursor_x;}
void Window::setCursorX(int new_cursor_x) {this->_cursor_x = new_cursor_x;}