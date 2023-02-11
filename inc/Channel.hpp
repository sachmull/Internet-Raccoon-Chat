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

#include <User.hpp>
#include <ModeDefine.hpp>

/**
 * Channels
**/
class Channel
{
	public:

	private:
		std::vector<User*>	registered_users_;
		std::vector<User*>	invited_users_;
		std::string			name_;
		size_t				mode_flags_;

	public:
		Channel(std::string name);
		~Channel();

		bool	RegisterUser(User* user);
		void	DeregisterUser(User* user);
		void	BroadcastMsg(std::vector<char> msg);

		std::string	GetName();
		void		SetMode(size_t flag);
		bool		AddInvitedUser(User* user);
		bool		IsUserInvited(User* user);
		bool		KickUser(User* user);

	private:
};

