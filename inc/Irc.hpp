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

	public:
		Irc();
		~Irc();

		static void	Recv(int fd);
		static void	Send(int fd);
		static void	Error(int fd);
		static void	ClosedClient(int fd);

		static void	DeleteCollector(int fd);
		static void EmptyDeleteCollector();


		static void	EventDistributor();

		// static void		CreateChannel(std::string channel_name);
		static bool		DeleteChannel(std::string channel_name);
		static Channel*	GetChannel(std::string channel_name);




		static bool	SendPrivateMsg(std::string nickname, std::vector<char>& msg);
		static void	AddUser(pollfd* poll_fd);
		static int	SearchUser(std::string& nickname);
	private:
};

