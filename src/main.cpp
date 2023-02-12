#include <Server.hpp>
#include <MsgParser.hpp>
#include <Debug.hpp>

int main(int argc, char* argv[])
{
	if (argc < 3)
		std::cout << "./ircserv [port > 0] [server]" << std::endl;

	int port = atoi(argv[1]);

	if (port <= 0)
		std::cout << "./ircserv [port > 0] [server]" << std::endl;
	try{
		Server Serv(port, 0, argv[2]);

		while(1)
		{
			Serv.PollEventHandler();
		}
	}catch(std::exception& e) {std::cerr << "main: " <<e.what() << std::endl;}
}
