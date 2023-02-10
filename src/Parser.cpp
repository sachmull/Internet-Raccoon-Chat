#include <Parser.hpp>

Parser::Parser(std::string input)
: input(input), pos(input.begin())
{}

message_t	Parser::parse_message() {
	message_t	message;

	message.prefix = parse_prefix();
	message.command = parse_command();

	return message;
}

prefix_t	Parser::parse_prefix() {
	prefix_t							prefix;
	std::pair<std::string, std::string>	substrs;
	std::string							prefix_literal;

	if (*pos == ':') {
		pos += input.find(' ');

		substrs = split(input, ' ');
		prefix_literal = substrs.first;
		input = substrs.second;

		substrs = split(prefix_literal, '@');
		prefix_literal = substrs.first;
		prefix.host = substrs.second;

		substrs = split(prefix_literal, '!');
		prefix.name = substrs.first;
		prefix.user = substrs.second;
	}

	return prefix;
}

std::string	Parser::parse_command() {
	std::string	command;

	// skip whitespace
	while (*pos == ' ') {
		++pos;
	}

	return command;
}

// std::string	Parser::parse_param() {
// 	std::string	param;

// 	while (*pos == ' ') {
// 		read_char();
// 	}

// 	if (*pos == ':') {
// 		++pos;
// 	}
// }

void	Parser::read_char() {
	++pos;
}

bool	Parser::is_letter() {
	if ((*pos >= 'a' && *pos <= 'z') || (*pos >= 'A' && *pos <= 'Z')) {
		return true;
	}

	return false;
}

bool	Parser::is_number() {
	if (*pos >= '0' && *pos <= '9') {
		return true;
	}

	return false;
}

bool	Parser::is_special() {
	if (*pos == '-' || *pos == '[' || *pos == ']' || *pos == '\\' || *pos == '`' || *pos == '^' || *pos == '{' || *pos == '}') {
		return true;
	}

	return false;
}

bool	Parser::is_chstring() {
	if (*pos != ' ' && *pos != '\a' && *pos != '\0' && *pos != '\r' && *pos != '\n' && *pos != ',') {
		return true;
	}

	return false;
}

bool	Parser::is_nonwhite() {
	if (*pos != ' ' && *pos != '\0' && *pos != '\r' && *pos != '\n') {
		return true;
	}

	return false;
}

std::pair<std::string, std::string>	Parser::split(std::string str, char c) {
	std::pair<std::string, std::string>	substrs;
	size_t	idx = str.find(c);

	if (idx == std::string::npos) {
		return std::make_pair(str, "");
	} else {
		return std::make_pair(str.substr(0, idx), str.substr(idx, std::string::npos));
	}
}
