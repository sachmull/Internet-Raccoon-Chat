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
#include "Socket.hpp"

#define POLLEVENTS (POLLIN|POLLOUT|POLLERR|POLLHUP|POLLNVAL)
/**
 * Sever handling every connection
**/
class Server
{
	private:
		static std::vector<pollfd>			poll_fds_;
		static Irc							irc_;
		static Socket						server_fd_;

	public:
		Server();
		~Server();

		// static void InitServer();
		static void	PollEventHandler();
		static void ErasePollFd(pollfd* fd);
	
	private:
		static void	ResetPollFdFlags();
};

