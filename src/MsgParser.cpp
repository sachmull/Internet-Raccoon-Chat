#include <MsgParser.hpp>
#include <MsgGeneration.hpp>
#include <Debug.hpp>


//	Message
Message::Message()
: command(), params(), is_complete(false)
{}

std::ostream&	operator<<(std::ostream& os, const Message& msg) {
	if (msg.is_complete) {
		os << "Prefix:" << std::endl;
		os << "name:\t\t" << msg.prefix.name << std::endl;
		os << "user:\t\t" << msg.prefix.user << std::endl;
		os << "host:\t\t" << msg.prefix.host << std::endl;
		os << std::endl;
		os << "Command:\t" << msg.command << std::endl;
		os << std::endl;
		os << "Parameters\t" << std::endl;

		for (std::vector<std::vector<std::string> >::const_iterator it = msg.params.begin(); it != msg.params.end(); ++it) {
			for (std::vector<std::string>::const_iterator jt = it->begin(); jt != it->end(); ++jt) {
				os << "| " << *jt << " |";
			}
			os << std::endl;
		}
	} else {
		os << "Message was not yet fully received" << std::endl;
	}
	os << std::endl;

	return os;
}


//	MsgParser
MsgParser::MsgParser()
: input(), pos(0)
{}

Message	MsgParser::parse(std::string to_parse) {
	Message	msg;

	input = to_parse;
	pos = 0;

	parse_prefix(msg.prefix);
	if (input[pos] == '\0') { //	Abort if message isnt complete
		return msg;
	}

	parse_command(msg.command);
	if (input[pos] == '\0') {	//	Abort if message isnt complete
		return msg;
	}

	parse_params(msg);

	// only lines that end with \r\n are considered complete messages
	// if (input[pos] != '\0' && input[pos] == '\r' && input[pos + 1] != '\0' && input[pos + 1] == '\n') {
	// 	msg.is_complete = true;
	// }
	msg.is_complete = true;

	return msg;
}

void	MsgParser::parse_prefix(Prefix& prefix) {
	if (input[pos] == ':') {
		// Parse name
		while (input[pos] != '\0' && input[pos] != '!' && input[pos] != '@' && input[pos] != ' ') {
			prefix.name += input[pos];
			++pos;
		}

		if (input[pos] == '\0' || input[pos] == ' ') {
			return ;
		}


		// Parse user
		if (input[pos] == '!') {
			while (input[pos] != '\0' && input[pos] != '@' && input[pos] != ' ') {
				prefix.user += input[pos];
				++pos;
			}

			if (input[pos] == '\0' || input[pos] == ' ') {
				return ;
			}
		}


		// Parse host
		if (input[pos] == '@') {
			while (input[pos] != '\0' && input[pos] != ' ') {
				prefix.host += input[pos];
				++pos;
			}

			// This if statement is not neccesary but left in for clarity
			if (input[pos] == '\0' || input[pos] == ' ') {
				return ;
			}
		}
	}
}

void	MsgParser::parse_command(t_command_type& command_type) {
	std::string	buf;

	skip_whitespace();

	while (input[pos] != '\0' && input[pos] != ' ') {
		buf += input[pos];
		++pos;
	}

	if (buf == "JOIN") {
		command_type = JOIN;
	} else if (buf == "PASS") {
		command_type = PASS;
	} else if (buf == "NICK") {
		command_type = NICK;
	} else if (buf == "USER") {
		command_type = USER;
	} else if (buf == "PRIVMSG") {
		command_type = PRIVMSG;
	} else if (buf == "OPER") {
		command_type = OPER;
	} else if (buf == "QUIT") {
		command_type = QUIT;
	} else if (buf == "KICK") {
		command_type = KICK;
	} else if (buf == "MODE") {
		command_type = MODE;
	} else if (buf == "INVITE") {
		command_type = INVITE;
	} else if (buf == "TOPIC") {
		command_type = TOPIC;
	}
	else {
		command_type = UNKNOWN;
	}
}

void	MsgParser::parse_params(Message& msg) {
	std::string	buf;

	// if this function gets called we aassume
	// pos == ' ' && pos != '\0'

	// read params until the end of the string or a potential end of line
	skip_whitespace();
	msg.params.push_back(std::vector<std::string>());
	while (input[pos] != '\0' && input[pos] != '\r') {
		if (input[pos] == ' ') {
			msg.params.back().push_back(buf);
			buf.clear();
			msg.params.push_back(std::vector<std::string>());
			skip_whitespace();
			if (input[pos] == ':') {	// tailing param
				++pos;	// jump over the :
				while (input[pos] != '\0' && input[pos] != '\r') {
					buf += input[pos];
					++pos;
				}

				break ;
			}
		} else if (input[pos] == ',') {
			msg.params.back().push_back(buf);
			buf.clear();
			++pos;	// jump over the ,
		} else {
			buf += input[pos];
			++pos;
		}
	}

	if (!buf.empty()) {
		msg.params.back().push_back(buf);
	}
}

void	MsgParser::skip_whitespace() {
	while (input[pos] != '\0' && input[pos] == ' ') {
		++pos;
	}
}


// Executor
void	Executor::execute(Message& msg, User& user) {
	if (user.IsAuthenticated() == false)
	{
	 	if (msg.command == PASS)
		{
			if(user.Authenticate(msg.params[0][0]) == false)
				user.WriteOutputBuff(err_passwd_mismatch());
		}
		else
			user.WriteOutputBuff(err_passwd_mismatch());
		return ;
	}
	if (msg.command == JOIN) {
		PRINTLN("JOIN");
		user.ConnectToChannel(msg.params[0][0]);
	} else if (msg.command == NICK) {
		PRINTLN("NICK");
		user.SetNickname(msg.params[0][0]);
	} else if (msg.command == USER) {
		PRINTLN("USER");
		user.SetUsername(msg.params[0][0]);
	} else if (msg.command == PRIVMSG) {
		user.SendMessage(msg.params[0], msg.params[1][0]);
	} else if (msg.command == OPER) {
		TODO("OPER");
	} else if (msg.command == QUIT) {
		std::cout << msg.params[0][0] << std::endl;
	} else if (msg.command == KICK) {
		user.KickUser(msg.params[0][0], msg.params[1][0]);
	} else if (msg.command == MODE) {
		user.SetMode(msg.params[0][0], msg.params[1][0]);
	} else if (msg.command == INVITE) {
		user.InviteUser(msg.params[1][0], msg.params[0][0]);
	} else if (msg.command == TOPIC) {
		TODO("TOPIC");
	} else if (msg.command == UNKNOWN) {
		TODO("UNKNOWN");
	}
	else {
		ERROR("This should be an unreachable statement");
	}
}
