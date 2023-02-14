#ifndef MSG_GENERATION
#define MSG_GENERATION

#include <string>

std::string	gen_welcome(std::string nickname);
std::string	gen_privmsg(std::string from, std::string to, std::string msg);
std::string	gen_kick(std::string from, std::string to, std::string channel, std::string comment);
std::string	gen_set_topic(std::string user, std::string channel, std::string topic);
std::string	gen_get_topic(std::string channel, std::string topic);

std::string	err_no_recipient(std::string command);
std::string	err_no_text_to_send(std::string nickname);
std::string	err_nickname_in_use(std::string nickname);
std::string	err_passwd_mismatch();
std::string	err_no_such_nick(std::string user, std::string nickname);
std::string	err_need_more_params(std::string user, std::string command);
std::string	err_not_on_channel(std::string user, std::string channel);
std::string	gen_no_topic(std::string channel);

#endif
