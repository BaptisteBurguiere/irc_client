#ifndef MODEL_H
# define MODEL_H

# include <vector>
# include <string>
# include <mutex>

typedef struct s_message
{
	int type;
	std::string message;
} t_message;

class Model
{
	public:
		Model(void);
		~Model(void);

		Model(const Model &other);
		Model &operator=(const Model &other);

		void init(void);

		std::string getInput(void);
		void setInput(std::string new_input);

		std::string getChannelName(void);
		void setChannelName(std::string new_channel_name);

		std::string getTopic(void);
		void setTopic(std::string new_topic);

		std::vector<std::string> getInputHistory(void);
		void addInputHistory(std::string new_input);

		std::vector<t_message> getChatHistory(void);
		void addChatHistory(t_message new_chat);

		void mutexLock(void);
		void mutexUnlock(void);

	private:
		std::string _input;
		std::string _channel_name;
		std::string _topic;
		std::vector<std::string> _input_history;
		std::vector<t_message> _chat_history;
		mutable std::mutex _mutex;
};

#endif