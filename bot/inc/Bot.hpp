#pragma once

#include <vector>
#include <map>
#include <exception>
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

		void	Recv();
		void	SendTime();

	private:
		Socket					socket_;
		std::vector<char>		input_buff_;
		std::vector<char>		output_buff_;

};
