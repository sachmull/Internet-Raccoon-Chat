#pragma once

#include <vector>
#include <map>
#include <exception>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>

#include "Server.hpp"
#include "Channel.hpp"


// #include "./Demultiplexer.hpp"

/**
 * IRC handling Channels
**/

class Irc
{
	private:
		static std::map<std::string, Channel>		channels_;
		static std::map<int, User>					conns_;
		static std::vector<User*>					conns_to_delete_;



	public:
		Irc();
		~Irc();

		static void	Recv(int fd);
		static void	Send(int fd);
		static void	Error(int fd);
		static void	ClosedClient(int fd);

		void	DeleteCollector(User* conn_identifier);
		void	EventDistributor();


	private:
};

