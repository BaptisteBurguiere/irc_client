#include <View.h>

View::View(void) {}

View::~View(void) {}

View::View(const View &other)
{
	this->_chat_window = other._chat_window;
	this->_input_window = other._input_window;
	std::unique_lock<std::mutex> lock_other(other._mutex);
}

View &View::operator=(const View &other)
{
	this->_chat_window = other._chat_window;
	this->_input_window = other._input_window;
	std::unique_lock<std::mutex> lock_this(this->_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock_other(other._mutex, std::defer_lock);
    std::lock(lock_this, lock_other);
    return *this;
}

void View::init(void)
{
	initscr();
	noecho();
	use_default_colors();

	start_color();
	init_pair(MESSAGE_COLOR, -1, -1);
	init_pair(SERVER_COLOR, COLOR_RED, -1);
	init_pair(DM_COLOR, COLOR_MAGENTA, -1);

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

void View::updateChatTitle(std::string channel_name, std::string topic)
{
	this->_chat_window.setTitle(channel_name + " - " + topic);
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

void View::mutexLock(void)
{
	this->_mutex.lock();
}

void View::mutexUnlock(void)
{
	this->_mutex.unlock();
}

void View::writeInChat(std::string message, int color)
{
	attron(COLOR_PAIR(color));
	for (int i = 0; i < (int)message.length(); ++i)
	{
		this->setCursorChat();
		if (this->increaseCursorChat())
			printw("%c", message[i]);
		else
			break;
	}
	attroff(COLOR_PAIR(color));
	this->chatNewLine();
	this->setCursorInput();
}

void View::printChar(char c)
{
	printw("%c", c);
}