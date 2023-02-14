#include "Bot.hpp"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "./ircserv [port > 0] [password]" << std::endl;
		return 0;
	}
	int port = atoi(argv[1]);
	if (port <= 0)
		std::cout << "./ircserv [port > 0] [password]" << std::endl;

	try{
		Bot bot(port);

		bot.InitBot(argv[2]);
		while(1)
		{
			bot.Recv();
			bot.SendTime();
		}
	}catch(std::exception& e) {std::cerr << "main: " <<e.what() << std::endl;}
}
