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

void	Irc::DeleteCollector(User* conn_identifier)
{
	conns_to_delete_.push_back(conn_identifier);
}

void	Irc::EmptyDeleteCollector()
{
	
}

bool CreateChannel(std::string name)
{
	return channels_.insert(std::pair(name, Channel()));
}

bool DeleteChannel(std::string name)
{
	return channels_.erase(name);
}
