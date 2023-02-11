#include <User.hpp>
#include <Server.hpp>


/* =================			Constructor/Deconstructor			================= */
User::User(pollfd* poll_fd) : client_closed_(false), is_operator_(false), channel_(NULL)
{
	socket_ = poll_fd;
	input_buff_.reserve(512);
}

User::~User()
{
	// Server::ErasePollFd(socket_);
}


/* =================			Poll Event Operations			================= */
bool	User::Recv()
{
	char		buff[512];
	ssize_t		c_received;


	c_received = recv(socket_->fd, &buff, 512, 0);
	if (c_received == -1)
		input_buff_.clear();
	else if (c_received == 0 && client_closed_)
		User::CloseConnection();
	else 
	{
		for (int i = 0; i < c_received; ++i)
			input_buff_.push_back(buff[i]);
		for (std::vector<char>::iterator it = input_buff_.begin(); it != input_buff_.end(); ++it) //test print function
			std::cout << *it;
		std::cout << std::endl;
	}
	// output_buff_.insert(output_buff_.begin(), input_buff_.begin(), input_buff_.end());
	return (true);
}

void	User::Send()
{
	int bytesRead = send(socket_->fd, static_cast<char*>(output_buff_.data()), output_buff_.size(), 0);
	if( bytesRead > 0)
		output_buff_.erase(output_buff_.begin(), output_buff_.begin()+ bytesRead);
	else
		;
}

void	User::ClosedClient()
{
	client_closed_ = true;
	User::Recv();
}

void	User::Error()
{
}

void	User::CloseConnection() { Irc::DeleteCollector(this->socket_->fd); }


/* =================			User Operations			================= */

void	User::SendPrivateMessage(std::string nickname, std::vector<char>& msg)
{
	if (Irc::SendPrivateMsg(nickname, msg) == false)
		; //fail error
}

void	User::ConnectToChannel(std::string channel_name)
{
	channel_ = Irc::GetChannel(channel_name);
	if (channel_ != NULL)
		channel_->RegisterUser(this);
}

void	User::DisconnectFromChannel()
{
	if (channel_ == NULL)
		return ;
	channel_->DeregisterUser(this);
	channel_ = NULL;
}

void	User::BroadcastMessage(std::vector<char>& msg)
{
	if (channel_ == NULL)
		return ;
	channel_->BroadcastMsg(msg);
}

void	User::ExitServer()
{
	Irc::DeleteCollector(this->socket_->fd); // maybe direct delete so that it cant pollin?
}

void	User::SetNickname(std::string nickname)
{
	//protect against double nicknames
	nickname_ = nickname;
}

void	User::SetUsername(std::string username)
{
	username_ = username;
}

void	User::GetOperator(std::string password)
{
	//check if password is true
	(void)password; //in irc
	is_operator_ = true;
}


/* =================			Operator Operations			================= */

// void	User::KickUser(std::string user)
// {
// 	if(is_operator_ == false)
// 		return ;
// }

// void	User::Mode(std::string user) //invite only
// {
// 	if(is_operator_ == false)
// 		return ;

// }

// void	User::InviteUser(std::string user)
// {
// 	if(is_operator_ == false)
// 		return ;

// }

// void	User::ChangeTopic(std::string new_topic)
// {
// 	if(is_operator_ == false)
// 		return ;

// }

/* =================				Helpers				================= */
void	User::WriteOutputBuff(std::vector<char>& msg)
{
	output_buff_.insert(output_buff_.end(), msg.begin(), msg.end());
}

void	User::SetOperator(bool set_as_op)
{
	is_operator_ = set_as_op;
}

bool	User::IsOperator()
{
	return is_operator_;
}

const std::string&	User::GetNickname() const
{
	return nickname_;
}