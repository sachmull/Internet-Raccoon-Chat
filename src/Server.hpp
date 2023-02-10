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

#include "Irc.hpp"
#include "User.hpp"


// #include "./Demultiplexer.hpp"

/**
 * Sever handling every connection
**/

class Server
{
	struct s_server_fd{
		int fd;
	};

	private:
		static std::vector<pollfd>			poll_fds_;
		static Irc							irc_;
		static s_server_fd					server_fd_;

	public:
		Server();
		~Server();

		// static void InitServer();
		static void	PollEventHandler();
	
	private:
};

