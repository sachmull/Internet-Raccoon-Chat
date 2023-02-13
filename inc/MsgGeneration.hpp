#ifndef MSG_GENERATION
#define MSG_GENERATION

#include <string>

std::string	gen_welcome(std::string nickname);
std::string	gen_privmsg(std::string& from, std::string& to, std::string& msg);

std::string	err_no_recipient(std::string command);
std::string	err_nickname_in_use(std::string nickname);

#endif
