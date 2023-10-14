#include <Model.h>

Model::Model(void) {}

Model::~Model(void) {}

Model::Model(const Model &other)
{
	this->_input = other._input;
	this->_channel_name = other._channel_name;
	this->_topic = other._topic;
	this->_input_history = other._input_history;
	this->_chat_history = other._chat_history;
	std::unique_lock<std::mutex> lock_other(other._mutex);
}

Model &Model::operator=(const Model &other)
{
	this->_input = other._input;
	this->_channel_name = other._channel_name;
	this->_topic = other._topic;
	this->_input_history = other._input_history;
	this->_chat_history = other._chat_history;
	std::unique_lock<std::mutex> lock_this(this->_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock_other(other._mutex, std::defer_lock);
    std::lock(lock_this, lock_other);
    return *this;
}

void Model::init(void)
{
	this->_input = "";
	this->_channel_name = "";
	this->_topic = "";
}

std::string Model::getInput(void) {return this->_input;}
void Model::setInput(std::string new_input) {this->_input = new_input;}

std::string Model::getChannelName(void) {return this->_channel_name;}
void Model::setChannelName(std::string new_channel_name) {this->_channel_name = new_channel_name;}

std::string Model::getTopic(void) {return this->_topic;}
void Model::setTopic(std::string new_topic) {this->_topic = new_topic;}

std::vector<std::string> Model::getInputHistory(void) {return this->_input_history;}
void Model::addInputHistory(std::string new_input) {this->_input_history.push_back(new_input);}

std::vector<t_message> Model::getChatHistory(void) {return this->_chat_history;}
void Model::addChatHistory(t_message new_chat) {this->_chat_history.push_back(new_chat);}

void Model::mutexLock(void)
{
	this->_mutex.lock();
}

void Model::mutexUnlock(void)
{
	this->_mutex.unlock();
}