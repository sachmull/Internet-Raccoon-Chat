#include <Parser.hpp>
#include <iostream>

Parser::Parser(std::string input)
: input(input), pos(0)
{}

message_t	Parser::parse_message() {
	message_t	message;

	message.prefix = parse_prefix();
	message.command = parse_command();
	while (input[pos] != '\r' && input[pos]) {
		message.params.push_back(parse_param());
	}

	return message;
}

prefix_t	Parser::parse_prefix() {
	prefix_t							prefix;
	std::pair<std::string, std::string>	substrs;
	std::string							prefix_literal;

	if (input[pos] == ':') {
		pos += input.find(' ');

		substrs = split(input, ' ');
		prefix_literal = substrs.first;
		// input = substrs.second;	// uncomment this to cut the prefix from the input str, invalidates pos

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

	skip_whitespace();

	while (input[pos] && input[pos] != ' ') {
		command += input[pos];
		++pos;
	}

	return command;
}

std::string	Parser::parse_param() {
	std::string	param;
	char		end_char = ' ';

	skip_whitespace();

	if (input[pos] == ':') {
		++pos;
		end_char = '\r';
	}

	while (input[pos] && input[pos] != end_char) {
		param += input[pos];
		++pos;
	}

	return param;
}

void	Parser::read_char() {
	++pos;
}

void	Parser::skip_whitespace() {
	while (input[pos] == ' ' && input[pos]) {
		read_char();
	}
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
