#include <MsgGeneration.hpp>
#include <Debug.hpp>

std::string	gen_welcome(std::string nickname) {
	return ":raccoon.chat 001 " + nickname + ":Welcome to the Internet Raccoon Network\r\n";
}

std::string	gen_privmsg(std::string from, std::string to, std::string msg) {
	std::string	output = ":" + from + " PRIVMSG " + to + " :" + msg + "\r\n";
	return output;
}

std::string	gen_kick(std::string from, std::string to, std::string channel, std::string comment = "") {
	return ":" + from + " KICK " + channel + " " + to + " :" + comment + "\r\n";
}

std::string	gen_set_topic(std::string user, std::string channel, std::string topic) {
	return ":" + user + " TOPIC " + channel + " " + topic + "\r\n";
}

std::string	gen_get_topic(std::string channel, std::string topic) {
	PRINTLN(topic);
	return ":raccoon.chat 332 " + channel + " :" + topic + "\r\n";
}

std::string	gen_no_topic(std::string channel) {
	return ":raccoon.chat 331 " + channel + " :No topic is set\r\n";
}

std::string	gen_now_operator(std::string user) {
	return ":raccoon.chat 381 " + user + " :Your are now an IRC operator\r\n";
}

std::string	err_no_recipient(std::string command) {
	return "411 :No recipient given (" + command + ")\r\n";
}

std::string	err_cannot_send_to_chan(std::string channel) {
	return "404 " + channel + " :Cannot send to channel\r\n";
}

std::string	err_no_text_to_send(std::string nickname) {
	return ":raccoon.chat 412 " + nickname + " :No text to send\r\n";
}

std::string	err_nickname_in_use(std::string nickname) {
	return ":raccoon.chat 433 " + nickname + " :Nickname is already in use\r\n";
}

std::string	err_passwd_mismatch() {
	return ":raccoon.chat 464 user :Password incorrect\r\n";
}

std::string	err_no_such_nick(std::string user, std::string nickname) {
	return ":raccoon.chat 401 " + user + " " + nickname + " :No such nick/channel\r\n";
}

std::string	err_need_more_params(std::string user, std::string command) {
	return ":raccoon.chat 461 " + user + " " + command + " :Not enough parameters\r\n";
}

std::string	err_not_on_channel(std::string user, std::string channel) {
	return ":raccoon.chat 442 " + user + " " + channel + " :You're not on that channel\r\n";
}

std::string	err_invite_only_chan(std::string channel) {
	return ":raccoon.chat 473 " + channel + " :Cannot join channel (+i)\r\n";
}

std::string	err_no_privileges(std::string user)	{
	return ":raccoon.chat 481 " + user + " :Permission Denied- You're not an IRC operator\r\n";
}
