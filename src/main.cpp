#include <Server.hpp>
#include <MsgParser.hpp>
#include <Debug.hpp>

int main(void)
{
	try{
		Server Serv(6664, 0);

		while(1)
		{
			Serv.PollEventHandler();
		}
	}catch(std::exception& e) {std::cerr << "main: " <<e.what() << std::endl;}
}
