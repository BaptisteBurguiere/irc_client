#include <View.h>

View::View(void) {}

View::~View(void) {}

void View::init(void)
{
	initscr();
	noecho();
	this->_input_window = Window(CHAT_WINDOW_SIZE, COLS, LINES - CHAT_WINDOW_SIZE, 0);
	this->_chat_window = Window(LINES - CHAT_WINDOW_SIZE - 1, COLS, 0, 0);
	this->_input_window.display();
	this->_chat_window.display();
}

void View::destroy(void)
{
	endwin();
}

void View::displayChat(void)
{
	this->_chat_window.display();
}

void View::displayInput(void)
{
	this->_input_window.display();
}

void View::updateChannelName(std::string name)
{
	this->_chat_window.setTitle(name);
	this->_chat_window.refreshWindow();
	this->_chat_window.display();
}

void View::setCursorInput(void)
{
	move(this->_input_window.getCursorY() + this->_input_window.getStartY(), this->_input_window.getCursorX() + this->_input_window.getStartX());
}

bool View::increaseCursorInput(void)
{
	return this->_input_window.increaseCursor();
}

bool View::decreaseCursorInput(void)
{
	return this->_input_window.decreaseCursor();
}

void View::setCursorChat(void)
{
	move(this->_chat_window.getCursorY() + this->_chat_window.getStartY(), this->_chat_window.getCursorX() + this->_chat_window.getStartX());
}

bool View::increaseCursorChat(void)
{
	return this->_chat_window.increaseCursor();
}

bool View::chatNewLine(void)
{
	if (this->_chat_window.getCursorY() < this->_chat_window.getHeight() - 2)
	{
		this->_chat_window.setCursorY(this->_chat_window.getCursorY() + 1);
		this->_chat_window.setCursorX(1);
	}
	else
		return false;
	return true;
}

void View::resize(void)
{
	clear();
	this->_input_window.setHeight(CHAT_WINDOW_SIZE);
	this->_input_window.setWidth(COLS);
	this->_input_window.setStartY(LINES - CHAT_WINDOW_SIZE);
	this->_input_window.setStartX(0);
	this->_input_window.setCursorY(1);
	this->_input_window.setCursorX(1);

	this->_chat_window.setHeight(LINES - CHAT_WINDOW_SIZE - 1);
	this->_chat_window.setWidth(COLS);
	this->_chat_window.setStartY(0);
	this->_chat_window.setStartX(0);
	this->_chat_window.setCursorY(1);
	this->_chat_window.setCursorX(1);

	this->_input_window.refreshWindow();
	this->_chat_window.refreshWindow();

	this->_input_window.display();
	this->_chat_window.display();
}