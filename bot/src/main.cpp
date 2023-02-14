#include "Bot.hpp"

int main()
{
	try{
		Bot bot(6667);

		bot.InitBot("ewf");
		while(1)
		{
			bot.Recv();
			bot.SendTime();
		}
	}catch(std::exception& e) {std::cerr << "main: " <<e.what() << std::endl;}
}