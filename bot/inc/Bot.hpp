#pragma once

#include <vector>
#include <map>
#include <exception>
#include <ctime>
#include <string>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>


#define POLLEVENTS (POLLIN|POLLOUT|POLLERR|POLLHUP|POLLNVAL)

class Bot
{
	struct Socket{
		int					fd;
		// struct sockaddr_in	address;
	};

	public:
		Bot(int port);
		~Bot();

		void	InitBot(std::string server_password);

		void	Recv();
		void	SendTime();
		void	GetTime();

		std::vector<char>	StrToOutputbuff(std::string s);


	private:
		Socket					socket_;
		std::vector<char>		input_buff_;
		std::vector<char>		output_buff_;

};
