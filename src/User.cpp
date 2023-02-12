#include <User.hpp>
#include <Server.hpp>
#include <sstream>


/* =================			Constructor/Deconstructor			================= */
User::User(pollfd* poll_fd) : client_closed_(false)
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
		User::ClosedConnection();
	else 
	{
		for (int i = 0; i < c_received; ++i)
			input_buff_.push_back(buff[i]);
		for (std::vector<char>::iterator it = input_buff_.begin(); it != input_buff_.end(); ++it) //test print function
			std::cout << *it;
		std::cout << std::endl;
	}
	MiniParse();
	// std::cout << "nickname: "<< nickname_ << std::endl;
	// output_buff_.insert(output_buff_.begin(), input_buff_.begin(), input_buff_.end());
	return (true);
}

void	User::Send()
{
	if (output_buff_.empty())
		return ;
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

void	User::ClosedConnection()
{
	Irc::DeleteUserFromChannels(this);
	Irc::DeleteCollector(this->socket_->fd);
	Server::ErasePollFd(socket_);
}

void	User::Error()
{
}




/* =================			User Operations			================= */

void	User::SendPrivateMessage(std::string nickname, std::vector<char>& msg)
{
	int	ret = Irc::SendPrivateMsg(nickname, msg);
	if (ret == -1)
		; //fail error
}

void	User::ConnectToChannel(std::string channel_name)
{
	try{
	Channel* channel = Irc::GetChannel(channel_name);
	if (channel == NULL)
		return ;
	channel->RegisterUser(this); //make it for multiple channels
	} catch (std::exception& e) { std::cerr << "co channel: " << e.what() << '\n'; }
}

void	User::DisconnectFromChannel(std::string channel_name)
{
	try{

	Channel* channel = Irc::GetChannel(channel_name);
	if (channel == NULL)
		return ;
	channel->DeregisterUser(this);
	} catch (std::exception& e) { std::cerr << "disco channel: " << e.what() << '\n'; }

}

void	User::BroadcastMessage(std::string channel_name, std::vector<char>& msg)
{
	try{
	Channel* channel = Irc::GetChannel(channel_name);
	if (channel == NULL)
		return ;
	channel->BroadcastMsg(msg);
	} catch (std::exception& e) { std::cerr << "broadcast: " << e.what() << '\n'; }

}

void	User::ExitServer()
{
	ClosedConnection(); // maybe direct delete so that it cant pollin?
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

/* =================			Operator Operations			================= */

void	User::SetMode(std::string channel_name) //invite only
{
	Channel* channel = Irc::GetChannel(channel_name);
	if (channel == NULL)
		return ;
	channel->SetMode(MODE_INVITE_ONLY, this);

}

void	User::InviteUser(std::string channel_name, std::string nickname)
{
	Channel* channel = Irc::GetChannel(channel_name);
	if (channel == NULL)
		return ;
	//send invite msg to user?
	//what happens if invited user changes nickname? atm saved as user handle not nickname specific
	channel->InviteUser(Irc::GetUserHandle(nickname), this);
}

void	User::KickUser(std::string channel_name, std::string nickname)
{
	Channel* channel = Irc::GetChannel(channel_name);
	if (channel == NULL)
		return ;
	channel->KickUser(Irc::GetUserHandle(nickname), this);
}

// void	User::ChangeTopic(std::string new_topic)
// {
// 	if (channel == NULL)
// {
	// std::cout << "no channel or is operator: " << is_operator_ << std::endl;
// 		return ;
// }

// }

/* =================				Helpers				================= */
int	User::WriteOutputBuff(std::vector<char>& msg)
{
	output_buff_.insert(output_buff_.end(), msg.begin(), msg.end());
	return (msg.size());
}

int	User::WriteOutputBuff(std::string msg)
{
	std::copy(msg.begin(), msg.end(), std::back_inserter(output_buff_));
	return msg.size();
}

const std::string&	User::GetNickname() const
{
	return nickname_;
}


/* =================				Testfunctions				================= */


void	User::MiniParse()
{
	try{

	std::string strmsg = "test msg\n";
	std::vector<char> testmsg;
	testmsg.insert(testmsg.begin(), strmsg.begin(), strmsg.end());
	strmsg = "broadcasting yes yes\n";
	std::vector<char> broadcastmsg;
	broadcastmsg.insert(broadcastmsg.begin(), strmsg.begin(), strmsg.end());
	std::string testchannel = "testchannel";
	if (input_buff_.at(0) == '1')
		nickname_ = VecToStr(input_buff_);
	if (input_buff_.at(0) == '2') //send priv message
		SendPrivateMessage(VecToStr(input_buff_), testmsg);
	// if (input_buff_.at(0) == '3') //get channel
	// 	WriteOutputBuff(channelname);
	if (input_buff_.at(0) == '4') //create/join channel
		ConnectToChannel("testchannel");
	if (input_buff_.at(0) == '5') //disconnect
		DisconnectFromChannel("testchannel");
	if (input_buff_.at(0) == '6') //broadcast_message
		BroadcastMessage("testchannel", broadcastmsg);
	if (input_buff_.at(0) == '7') //sets to invite only
		SetMode("testchannel");
	if (input_buff_.at(0) == '8')
		InviteUser("testchannel", VecToStr(input_buff_));
	if (input_buff_.at(0) == '9')
		KickUser("testchannel", VecToStr(input_buff_));
	input_buff_.clear();
	}catch (std::exception& e) { std::cerr << "miniparse: " << e.what() << '\n'; }
}

std::string User::VecToStr(std::vector<char>& msg)
{
	std::ostringstream	stream;

	for (std::vector<char>::iterator it = msg.begin() + 1; it != msg.begin() + 4; ++it)
	{
		stream << *it;
	}
	return (stream.str());
}
std::vector<char> User::StrToVec(std::string s)
{
	std::vector<char> v;
	std::copy(s.begin(), s.end(), std::back_inserter(v));
	return v;
}