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


// #include "./Demultiplexer.hpp"

/**
 * IRC handling Channels
**/

class Irc
{
	private:
		std::map<std::string, Channel>		conns_;
		Server*								server_handle_;

	public:
		Irc(Server* server_handle);
		~Irc();


		void	CreateServers(); // call config file parser and create servers
		void	PollEventHandler();
	
	private:
};

