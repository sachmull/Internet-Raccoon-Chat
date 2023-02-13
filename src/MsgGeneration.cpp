#include <MsgGeneration.hpp>

std::string	gen_welcome(std::string nickname) {
	return ":raccoon.chat 001 " + nickname + ":Welcome to the Internet Raccoon Network\r\n";
}

std::string	gen_privmsg(std::string& from, std::string& to, std::string& msg) {
	std::string	output = ":" + from + " PRIVMSG " + to + " :" + msg + "\r\n";
	return output;
}

std::string	err_no_recipient(std::string command) {
	return "411 :No recipient given (" + command + ")\r\n";
}

std::string	err_cannot_send_to_chan(std::string channel) {
	return "404 " + channel + " :Cannot send to channel\r\n";
}

std::string	err_no_text_to_send() {
	return "412 :No text to send\r\n";
}

std::string	err_nickname_in_use(std::string nickname) {
	return ":raccoon.chat 433 " + nickname + " :Nickname is already in use\r\n";
}

std::string	err_passwd_mismatch() {
	return ":raccoon.chat 464 user :Password incorrect\r\n";
}
