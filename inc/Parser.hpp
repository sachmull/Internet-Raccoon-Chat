#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

typedef struct {
	std::string	name;
	std::string	user;
	std::string	host;
}	prefix_t;

typedef struct message_s {
	prefix_t					prefix;
	std::string					command;
	std::vector<std::string>	params;
}	message_t;

class Parser {
	private:
		std::string				input;
		std::size_t	pos;

	public:
		Parser(std::string input);
		message_t	parse_message();

	private:
		prefix_t	parse_prefix();
		std::string	parse_command();
		std::string	parse_param();

		void	read_char();

		void	skip_whitespace();
		std::pair<std::string, std::string>	split(std::string str, char c);
};

#endif
