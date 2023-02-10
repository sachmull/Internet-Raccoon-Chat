#include <Server.hpp>

int main(void)
{
	Server Serv(3306, 0);
	while(1)
	{
		Serv.PollEventHandler();
	}
}