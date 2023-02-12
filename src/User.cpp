#include <User.hpp>
#include <Server.hpp>
#include <sstream>
#include <MsgParser.hpp>


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
	// MiniParse();
	MsgParser	parser;
	Message		msg;
	Executor	exe;
	std::string	input(input_buff_.begin(), input_buff_.end());
	while (input.find('\n') != std::string::npos) {
		// outputs the current line that will be fed into the parser
		// std::cout << "to_parse:" << input.substr(0, input.find('\n')) << std::endl;
		if ((msg = parser.parse(input.substr(0, input.find('\n')))).is_complete) {
			// std::cout << msg;
			exe.execute(msg, *this);
		}
		input = input.substr(input.find('\n') + 1, input.length());
	}
	input_buff_.clear();

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
	User::CloseConnection();
}

void	User::Error()
{
}

void	User::CloseConnection()
{
	DisconnectFromChannel();
	Irc::DeleteCollector(this->socket_->fd);
	Server::ErasePollFd(socket_);
	
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
	channel_ = Irc::GetChannel(channel_name);
	if (channel_ != NULL && (channel_->RegisterUser(this) == false))
		channel_ = NULL;
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
	CloseConnection(); // maybe direct delete so that it cant pollin?
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

void	User::SetMode() //invite only
{
	if(is_operator_ == false)
		return ;
	channel_->SetMode(MODE_INVITE_ONLY);

}

void	User::InviteUser(std::string nickname)
{
	if(is_operator_ == false || channel_ == NULL)
		return ;
	//send invite msg to user?
	//what happens if invited user changes nickname? atm saved as user handle not nickname specific
	channel_->AddInvitedUser(Irc::GetUserHandle(nickname));
}

void	User::KickUser(std::string nickname)
{
	if(is_operator_ == false || channel_ == NULL)
		return ;
	channel_->KickUser(Irc::GetUserHandle(nickname));
}

// void	User::ChangeTopic(std::string new_topic)
// {
// 	if(is_operator_ == false)
// 		return ;

// }

/* =================				Helpers				================= */
int	User::WriteOutputBuff(std::vector<char>& msg)
{
	output_buff_.insert(output_buff_.end(), msg.begin(), msg.end());
	return (msg.size());
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


/* =================				Testfunctions				================= */


void	User::MiniParse()
{
	std::string strmsg = "test msg\n";
	std::vector<char> testmsg;
	testmsg.insert(testmsg.begin(), strmsg.begin(), strmsg.end());
	strmsg = "broadcasting yes yes\n";
	std::vector<char> broadcastmsg;
	broadcastmsg.insert(broadcastmsg.begin(), strmsg.begin(), strmsg.end());
	std::vector<char> channelname;
	if (channel_ != NULL)
		channelname = StrToVec(channel_->GetName());
	else
		channelname.push_back('0');

	if (input_buff_.at(0) == '1')
		nickname_ = VecToStr(input_buff_);
	if (input_buff_.at(0) == '2') //send priv message
		SendPrivateMessage(VecToStr(input_buff_), testmsg);
	if (input_buff_.at(0) == '3') //get channel
		WriteOutputBuff(channelname);
	if (input_buff_.at(0) == '4') //create/join channel
		ConnectToChannel("testchannel");
	if (input_buff_.at(0) == '5') //disconnect
		DisconnectFromChannel();
	if (input_buff_.at(0) == '6') //broadcast_message
		BroadcastMessage(broadcastmsg);
	if (input_buff_.at(0) == '7') //sets to invite only
		SetMode();
	if (input_buff_.at(0) == '8')
		InviteUser(VecToStr(input_buff_));
	if (input_buff_.at(0) == '9')
		KickUser(VecToStr(input_buff_));
	input_buff_.clear();
}

std::string User::VecToStr(std::vector<char>& msg)
{
	std::ostringstream	stream;

	for (std::vector<char>::iterator it = msg.begin() + 1; it != msg.end(); ++it)
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