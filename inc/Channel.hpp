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
		User*				operator_;
		std::string			name_;
		size_t				mode_flags_;

	public:
		Channel(std::string name);
		~Channel();

		bool	RegisterUser(User* user);
		bool	DeregisterUser(User* user);
		void	BroadcastMsg(std::vector<char> msg);
		void	BroadcastMsg(std::string msg);

		std::string	GetName();
		bool		InviteUser(User* new_user, User* commanding_user);
		bool		KickUser(User* kick_user, User* commanding_user);
		void		AddMode(size_t flag, User* commanding_user);
		void		RemoveMode(size_t flag, User* commanding_user);

		bool		IsUserRegistered(User* user);

		bool		gets_deleted;
	private:
		bool		IsOperator(User* user);
		bool		IsUserInvited(User* user);


};

