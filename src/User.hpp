#pragma once

#include <poll.h>
#include <string>
#include <vector>
#include <sys/socket.h>

#include "Server.hpp"
#include "Irc.hpp"
#include "Socket.hpp"

/**
 * User handling
**/
class User
{
	private:
		std::vector<char>	input_buff_;
		std::vector<char>	output_buff_;
		bool				client_closed_;
		pollfd*				socket_;

	public:
		User(pollfd* poll_fd);
		~User();

		bool	Recv(); // gets message from client -> reads in input buff
		void	Send(); // sends message to client -> reads from output buff
		void	ParseInputBuff(); //server parser
		void	ClosedClient(); // sets client to closed and trys to receive data until 0
		void	Error();

		void	WriteOutputBuff(std::vector<char>& msg);


	private:
		void	CloseConnection(); // closes connection and adds it to delete collector of multiplexer
};

bool operator==(const User& lhs, const User& rhs);
