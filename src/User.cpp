#include "./User.hpp"

User::User(pollfd* poll_fd) : client_closed_(false)
{
	socket_ = poll_fd;
	input_buff_.reserve(512);
}

User::~User()
{
	Server::ErasePollFd(socket_);
}


bool	User::Recv()
{
	std::string	received;
	ssize_t		c_received;

	c_received = recv(socket_->fd, input_buff_.data() + input_buff_.size(), input_buff_.capacity() - input_buff_.size(), NULL);
	if (c_received == -1)
		input_buff_.clear();
	if (c_received == 0 && client_closed_)
		User::CloseConnection();
}

void	User::Send()
{
	
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


void	User::WriteOutputBuff(std::vector<char>& msg)
{
	output_buff_.insert(output_buff_.end(), msg.begin(), msg.end());
}
