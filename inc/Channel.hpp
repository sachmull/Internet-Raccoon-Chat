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
 * Channel handles which user is allowed to do what in the channel
**/
class Channel
{
	public:
		Channel(std::string name);
		~Channel();

/* =================			Channel Operations			================= */
		bool		RegisterUser(User* user);
		bool		DeregisterUser(User* user);
		std::string	GetName();
		bool		IsUserRegistered(User* user);

/* =================			User Operations			================= */
		void		BroadcastMsg(User& user, std::string msg);
		void		GetTopic(User* commanding_user);

/* =================			Operator Operations			================= */
		bool		KickUser(User* kick_user, User* commanding_user);
		void		AddMode(size_t flag, User* commanding_user);
		void		RemoveMode(size_t flag, User* commanding_user);
		void		SetTopic(std::string& new_topic, User* commanding_user);
		bool		InviteUser(User* new_user, User* commanding_user);

/* =================			Garbage Collector			================= */
		bool		gets_deleted;

/* =================			Private Helpers			================= */
	private:
		bool		IsOperator(User* user);
		bool		IsUserInvited(User* user);

/* =================			Private Variables			================= */
	private:
		std::vector<User*>	registered_users_;
		std::vector<User*>	invited_users_;
		User*				operator_;
		std::string			name_;
		std::string			topic_;
		size_t				mode_flags_;


};

