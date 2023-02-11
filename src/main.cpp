#include <Server.hpp>
#include <MsgParser.hpp>

int main(void)
{
	Message	msg;
	MsgParser	parser;

	msg = parser.parse(":name");
	std::cout << msg;

	msg = parser.parse(":name!user@host JOIN channel1,channel2 :key1 key2\r\n");
	std::cout << msg;

	Server Serv(6667, 0);
	while(1)
	{
		Serv.PollEventHandler();
	}
}
