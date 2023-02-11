#pragma once

#include <poll.h>
#include <string>
#include <vector>
#include <sys/socket.h>

#include <ModeDefine.hpp>

class Channel;

/**
 * User handling
**/
class User
{
	private:
		std::vector<char>	input_buff_;
		std::vector<char>	output_buff_;
		bool				client_closed_;
		bool				is_operator_;
		std::string			username_;
		std::string			nickname_;
		pollfd*				socket_;
		Channel*			channel_;

	public:
		User(pollfd* poll_fd);
		~User();

		bool	Recv(); // gets message from client -> reads in input buff
		void	Send(); // sends message to client -> reads from output buff
		void	ParseInputBuff(); //server parser
		void	ClosedClient(); // sets client to closed and trys to receive data until 0
		void	ClosedConnection();
		void	Error();

/* =================			User Operations			================= */

		void	SendPrivateMessage(std::string nickname, std::vector<char>& msg);
		void	BroadcastMessage(std::vector<char>& msg);
		void	DisconnectFromChannel();
		void	ConnectToChannel(std::string channel_name);
		void	ExitServer();
		void	SetNickname(std::string nickname);
		void	SetUsername(std::string username);
		void	GetOperator(std::string password);

/* =================			Operator Operations			================= */

		void	SetMode(); //invite only
		void	InviteUser(std::string nickname);
		void	KickUser(std::string nickname);
		// void	ChangeTopic(std::string new_topic);

/* =================				Getter				================= */
		const std::string&	GetNickname() const;

/* =================				Setter				================= */
		void	SetOperator(bool set_as_op);
	
/* =================				Helpers				================= */
		int		WriteOutputBuff(std::vector<char>& msg);
		bool	IsOperator();

	private:

		void	CloseConnection(); // closes connection and adds it to delete collector of multiplexer


/* =================				Testfunctions				================= */
		void	MiniParse();

		std::string VecToStr(std::vector<char>& msg);
		std::vector<char> StrToVec(std::string s);


};

// bool operator==(const User& lhs, const User& rhs);
