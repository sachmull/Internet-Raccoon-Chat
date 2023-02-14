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
	public:
		Bot(int port);
		~Bot();

		void	InitBot(std::string server_password);

/* =================			IO			================= */

		void	Recv();
		void	SendTime();

/* =================			OutputBuffWriter			================= */

		void				GetTime();
		std::string			GetNameFromInput();



	private:
		int					fd_;
		std::vector<char>	input_buff_;
		std::vector<char>	output_buff_;
};
