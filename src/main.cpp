#include <Server.hpp>

int main(void)
{
	Server Serv(6667, 0);
	while(1)
	{
		Serv.PollEventHandler();
	}
}
