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



/**
 * Channels
**/
class Channel
{
	private:
		std::vector<User*>	registered_users_;

	public:
		Channel();
		~Channel();

		void	RegisterUser(User* user);
		void	DeregisterUser(User* user);
		void	BroadcastMsg(std::vector<char> msg);

	private:
};

