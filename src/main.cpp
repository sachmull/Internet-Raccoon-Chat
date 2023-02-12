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

	Server Serv(6667, 0);
	while(1)
	{
		Serv.PollEventHandler();
	}
}
