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
		std::string			username_;
		std::string			nickname_;
		pollfd*				socket_;

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
		void	BroadcastMessage(std::string channel_name, std::vector<char>& msg);
		void	DisconnectFromChannel(std::string channel_name);
		void	ConnectToChannel(std::string channel_name);
		void	ExitServer();
		void	SetNickname(std::string nickname);
		void	SetUsername(std::string username);

/* =================			Operator Operations			================= */

		void	SetMode(std::string channel_name); //invite only
		void	InviteUser(std::string channel_name, std::string nickname);
		void	KickUser(std::string channel_name, std::string nickname);
		// void	ChangeTopic(std::string new_topic);

/* =================				Getter				================= */
		const std::string&	GetNickname() const;


/* =================				Helpers				================= */
		int		WriteOutputBuff(std::vector<char>& msg);
		int		WriteOutputBuff(std::string msg);

	private:

/* =================				Testfunctions				================= */
		void	MiniParse();

		std::string VecToStr(std::vector<char>& msg);
		std::vector<char> StrToVec(std::string s);


};

// bool operator==(const User& lhs, const User& rhs);
