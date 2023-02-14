#include <Irc.hpp>
#include <iostream>
#include <MsgGeneration.hpp>
#include <Debug.hpp>
// #include <Server.hpp>

std::map<std::string, Channel>		Irc::channels_;
std::map<int, User>					Irc::conns_;
std::vector<int>					Irc::conns_to_delete_;
std::string							Irc::password_;

Irc::Irc()
{
}

Irc::~Irc()
{
	
}

void	Irc::SetPassword(std::string password)
{
	password_ = password;
}
/* =================			Poll Events			================= */

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

void	Irc::ClosedConnection(int fd)
{
	conns_.at(fd).ClosedConnection();
}


Channel* Irc::CreateChannel(std::string channel_name)
{
	channel_iterator	new_channel;

	new_channel = channels_.insert(std::pair<std::string, Channel>(channel_name, Channel(channel_name))).first;
	return &new_channel->second;
}

/* =================			Channel Operations			================= */

void Irc::DeleteUnusedChannels()
{
	for(channel_iterator it = channels_.begin(); it != channels_.end();)
	{
		if (it->second.gets_deleted)
		{
			std::cout << "channel erased: "<< it->second.GetName() << std::endl;
			channels_.erase(it);
			it = channels_.begin();
		}
		else
			++it;
	}
}

// gets channel, creates channel if channel does not exist
Channel* Irc::GetChannel(std::string channel_name)
{
	channel_iterator it = channels_.find(channel_name);
	if (it == channels_.end())
		return NULL;
	else
		return &(it->second);
}


/* =================			User Operations			================= */

//adds user to server conn vector
void Irc::AddUser(pollfd poll_fd)
{
	conns_.insert(std::pair<int, User>(poll_fd.fd, User(poll_fd)));
	std::cout << "Irc Adduser fd: " << poll_fd.fd << std::endl;
}

//returns -1 when no user found
int Irc::GetUserFd(std::string& nickname)
{
	for(conn_iterator it = conns_.begin(); it != conns_.end(); ++it)
	{
		if (nickname == it->second.GetNickname())
			return(it->first);
	}
	return -1;
}

// returns user* if user is on server, NULL if not
User* Irc::GetUserHandle(std::string& nickname)
{
	for(conn_iterator it = conns_.begin(); it != conns_.end(); ++it)
	{
		if (nickname == it->second.GetNickname())
			return &(it->second);
	}
	return NULL;
}

int Irc::SendPrivateMsg(std::string from, std::string nickname, std::string msg) //msg to user
{
	int	fd = GetUserFd(nickname);
	if (fd == -1)
		return -1;
	std::string	new_msg = ":" + from + " PRIVMSG " + nickname + " :" + msg + "\r\n";
	return conns_.at(fd).WriteOutputBuff(new_msg);
}

void Irc::DistributeMsg(std::vector<std::string> names, std::string msg, User* user)
{
	for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); ++it)
	{
		if (SendPrivateMsg(user->GetNickname(), *it, msg) != -1)
			; //inefficient because of string copying
		else 
		{
			Channel* channel = GetChannel(*it);
			if (channel != NULL && channel->IsUserRegistered(user))
				channel->BroadcastMsg(*user, msg);
			else if (channel == NULL) {
				PRINTLN("no such nickname");
				user->WriteOutputBuff(err_no_such_nick(user->GetNickname(), *it));
			}
		}
	}
}

bool	Irc::IsNicknameUsed(std::string& nickname)
{
	for(conn_iterator it = conns_.begin(); it != conns_.end(); ++it)
	{
		if (it->second.GetNickname() == nickname)
			return true;
	}
	return false;
}


/* =================			GarbageCollector			================= */



/* =================			Password			================= */

bool Irc::CompareServerPassword(std::string password)
{
	if (password == password_)
		return true;
	return false;
}

void Irc::SetServerPassword(std::string password)
{
	password = password_;
}

/* =================			Irc Events			================= */

void Irc::GetReady()
{
	EmptyDeleteCollector();
	DeleteUnusedChannels();
}


/* =================			GarbageCollector			================= */

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

void	Irc::DeleteUserFromChannels(User *user)
{
	for(channel_iterator it = channels_.begin(); it != channels_.end(); ++it)
	{
		it->second.DeregisterUser(user);
	}
}
