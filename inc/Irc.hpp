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

#include <Channel.hpp>
// #include <User.hpp>

/**
 * IRC handling Channels
**/
class Irc
{
	private:
		typedef std::map<std::string, Channel>::iterator	channel_iterator;
		typedef std::map<int, User>::iterator				conn_iterator;

	private:
		static std::map<std::string, Channel>		channels_;
		static std::map<int, User>					conns_;
		static std::vector<int>						conns_to_delete_;
		static std::vector<Channel*>				channels_to_delete_;
		static std::string							password_;


	public:
		Irc();
		~Irc();
		static void	SetPassword(std::string password);
		static void	EventDistributor();

/* =================			Poll Events			================= */
		static void	Recv(int fd);
		static void	Send(int fd);
		static void	Error(int fd);
		static void	ClosedClient(int fd);
		static void	ClosedConnection(int fd);


/* =================			Channel Operations			================= */

		static void		DeleteUnusedChannels();
		static Channel*	GetChannel(std::string channel_name);
		static Channel* CreateChannel(std::string channel_name);




/* =================			User Operations			================= */
		static int		SendPrivateMsg(std::string from, std::string nickname, std::string msg);
		static void		DistributeMsg(std::vector<std::string> names, std::string msg, User* user);

		static void		AddUser(pollfd poll_fd);
		static int		GetUserFd(std::string& nickname);
		static User*	GetUserHandle(std::string& nickname);
		static bool		IsNicknameUsed(std::string& nickname);

/* =================			Irc Events			================= */
		static void GetReady();

/* =================			Password			================= */
		static bool CompareServerPassword(std::string password);
		static void SetServerPassword(std::string password);

/* =================			GarbageCollector			================= */

		static void	DeleteCollector(int fd);
		static void EmptyDeleteCollector();
		static void	DeleteUserFromChannels(User* user);

	private:
};

