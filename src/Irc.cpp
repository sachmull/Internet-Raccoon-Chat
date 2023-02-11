#include <Irc.hpp>
#include <iostream>
// #include <Server.hpp>

std::map<std::string, Channel>		Irc::channels_;
std::map<int, User>					Irc::conns_;
std::vector<int>					Irc::conns_to_delete_;

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

// void Irc::CreateChannel(std::string channel_name)
// {
// 	channels_.insert(std::pair<std::string, Channel>(channel_name, Channel()));
// }

bool Irc::DeleteChannel(std::string channel_name)
{
	return channels_.erase(channel_name);
}

Channel* Irc::GetChannel(std::string channel_name)
{
	 

	std::pair<channel_iterator, bool>	channel;

	channel = channels_.insert(std::pair<std::string, Channel>(channel_name, Channel()));
	return &(channel.first->second);
}

void Irc::AddUser(pollfd* poll_fd)
{
	conns_.insert(std::pair<int, User>(poll_fd->fd, User(poll_fd)));
}


bool Irc::SendPrivateMsg(std::string nickname, std::vector<char>& msg)
{
	int	fd = SearchUser(nickname);
	if (fd == -1)
		return false;
	conns_.at(fd).WriteOutputBuff(msg);
	return true;
}

int Irc::SearchUser(std::string& nickname)
{
	for(conn_iterator it = conns_.begin(); it != conns_.end(); ++it)
	{
		if (nickname == it->second.GetNickname())
			return(it->first);
	}
	return (-1);
}



