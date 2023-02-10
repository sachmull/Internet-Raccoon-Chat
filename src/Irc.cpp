#include "./Irc.hpp"

Irc::Irc()
{
}

Irc::~Irc()
{
	
}

void	Irc::Recv(int fd)
{
	conns_.at(fd).Recv();
}

void	Irc::Send(int fd)
{
	conns_.at(fd).Send();
}

void	Irc::Error(int fd)
{
	conns_.at(fd).Error();
}

void	Irc::ClosedClient(int fd)
{
	conns_.at(fd).ClosedClient();
}

void	Irc::DeleteCollector(int fd)
{
	conns_to_delete_.push_back(fd);
}

void	Irc::EmptyDeleteCollector()
{
	for(std::vector<int>::iterator it = conns_to_delete_.begin(); it != conns_to_delete_.end(); ++it)
	{
		conns_.erase(*it);
	}
	conns_to_delete_.clear();
}

void Irc::CreateChannel(std::string name)
{
	channels_.insert(std::pair<std::string, Channel>(name, Channel()));
}

bool Irc::DeleteChannel(std::string name)
{
	return channels_.erase(name);
}

void Irc::AddUser(pollfd* poll_fd)
{
	conns_.insert(std::pair<int, User>(poll_fd->fd, User(poll_fd)));
}
