#include <Server.hpp>
#include <MsgParser.hpp>
#include <Debug.hpp>

int main(void)
{
	Message	msg;
	MsgParser	parser;

	msg = parser.parse(":name");
	std::cout << msg;

	msg = parser.parse("PRIVMSG hey\r\n");
	std::cout << msg;

	try{
		Server Serv(6664, 0);

		while(1)
		{
			Serv.PollEventHandler();
		}
	}catch(std::exception& e) {std::cerr << "main: " <<e.what() << std::endl;}
}
