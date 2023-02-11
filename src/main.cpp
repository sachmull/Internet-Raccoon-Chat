#include <Server.hpp>

int main(void)
{
	try{
		Server Serv(6666, 0);

		while(1)
		{
			Serv.PollEventHandler();
		}
	}catch(std::exception& e) {std::cerr << "main: " <<e.what() << std::endl;}
}
