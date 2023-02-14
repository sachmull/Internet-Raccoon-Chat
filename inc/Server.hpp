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

#include <Irc.hpp>

#define POLLEVENTS (POLLIN|POLLOUT|POLLERR|POLLHUP|POLLNVAL)
/**
 * Static Server to handle every connection and poll related command
**/
class Server
{
	public:
		struct Socket{
			int					fd;
			struct sockaddr_in	address;
		};

	public:
		Server(int port, int queue_length, std::string password);
		~Server();

/* =================				Server Actions				================= */
		static void	PollEventHandler();
		static void ErasePollFd(int	fd);
	
/* =================				Private Helpers				================= */
	private:
		static void	ResetPollFdFlags();
		static void Accept();
		static void AddConnection(struct pollfd poll_fd, struct sockaddr addr);

/* =================				Private Variables				================= */
	private:
		static std::vector<pollfd>			poll_fds_;
		static Irc							irc_;
		static Socket						socket_;
};

