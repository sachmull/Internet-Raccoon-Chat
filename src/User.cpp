#include <User.hpp>
#include <Server.hpp>

User::User(pollfd* poll_fd) : client_closed_(false)
{
	socket_ = poll_fd;
	input_buff_.reserve(512);
}

User::~User()
{
	// Server::ErasePollFd(socket_);
}


bool	User::Recv()
{
	std::string	received;
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
	return (true);
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
