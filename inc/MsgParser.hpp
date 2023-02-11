#ifndef MSG_PARSER_HPP
#define MSG_PARSER_HPP

#include <iostream>

#include <vector>
#include <string>

typedef enum e_command_type {
	JOIN,
	PASS,
	NICK,
	USER,
	PRIVMSG,
	OPER,
	QUIT,
	KICK,
	MODE,
	INVITE,
	TOPIC,
	UNKNOWN,
}	t_command_type;

class Prefix {
	public:
		std::string	name;
		std::string	user;
		std::string	host;
};

class Message {
	public:
		Prefix									prefix; //	prefix
		t_command_type							command; //	command
		std::vector<std::vector<std::string> >	params; //	params
		bool									is_complete;	// allows if ((msg = parser.parse(input)).is_complete)

	public:
		Message();

	friend std::ostream&	operator<<(std::ostream& os, const Message& msg);
};

class MsgParser {
	private:
		std::string				input;
		std::string::size_type	pos;

	public:
		MsgParser();
		Message	parse(std::string input);

	private:
		void	parse_prefix(Prefix& prefix);
		void	parse_command(t_command_type& command_type);
		void	parse_params(Message& msg);

		void	skip_whitespace();
};

class Executor {
	public:
		void	execute(Message& msg);
};

#endif
