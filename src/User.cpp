#include <User.hpp>
#include <Server.hpp>
#include <sstream>
#include <MsgParser.hpp>
#include <MsgGeneration.hpp>


/* =================			Constructor/Deconstructor			================= */
User::User(pollfd poll_fd) : client_closed_(false), is_registered_(false), is_authenticated_(false)
{
	socket_ = poll_fd;
	input_buff_.reserve(512);
}

User::~User()
{
}


/* =================			Poll Event Operations			================= */
bool	User::Recv()
{
	char		buff[512];
	ssize_t		c_received;


	c_received = recv(socket_.fd, &buff, 512, 0);
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
	MsgParser	parser;
	Message		msg;
	Executor	exe;
	std::string	input(input_buff_.begin(), input_buff_.end());
	while (input.find('\n') != std::string::npos) {
		if ((msg = parser.parse(input.substr(0, input.find('\n')))).is_complete) {
			exe.execute(msg, *this);
		}
		input = input.substr(input.find('\n') + 1, input.length());
	}
	input_buff_.clear();
	return (true);
}

void	User::Send()
{
	if (output_buff_.empty())
		return ;
	int bytesRead = send(socket_.fd, static_cast<char*>(output_buff_.data()), output_buff_.size(), 0);
	if( bytesRead > 0)
		output_buff_.erase(output_buff_.begin(), output_buff_.begin()+ bytesRead);
	else
		;
}

void	User::ClosedClient()
{
	std::cout << "closed client: " << GetNickname() << std::endl;
	client_closed_ = true;
	User::Recv();
}

void	User::ClosedConnection()
{
	std::cout << "closed connection: " << GetNickname() << std::endl;
	Irc::DeleteUserFromChannels(this);
	Irc::DeleteCollector(this->socket_.fd);
}

void	User::Error()
{
	std::cout << "some error in connection to client orccured: " << GetNickname() << std::endl;
}

/* =================			User Operations			================= */

void	User::SendMessage(std::vector<std::string>& names, std::string& msg)
{
	Irc::DistributeMsg(names, msg, this);
}

void	User::ConnectToChannel(std::string channel_name)
{
	if (channel_name.front() == '#') {
		try{
		Channel* channel = Irc::CreateChannel(channel_name);
		if (channel == NULL)
			return ;
		channel->RegisterUser(this); //make it for multiple channels
		} catch (std::exception& e) { std::cerr << "co channel: " << e.what() << '\n'; }
	}
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

void	User::ExitServer()
{
	ClosedConnection(); // maybe direct delete so that it cant pollin?
}

void	User::SetNickname(std::string nickname)
{
	//protect against double nicknames
	if (Irc::IsNicknameUsed(nickname) == true)
	{
		// WriteOutputBuff("nickname already in use\n");
		WriteOutputBuff(err_nickname_in_use(nickname));
		return ;
	}
	std::cout << "set nickname: " << nickname << std::endl;
	nickname_ = nickname;
	if (is_authenticated_ == false && !username_.empty())
			is_authenticated_ = true;
}

void	User::SetUsername(std::string username)
{
	username_ = username;
	std::cout << "set username: " << username << std::endl;
	if (is_authenticated_ == false && !nickname_.empty())
		is_authenticated_ = true;
}

bool	User::Register(std::string password)
{
	if (Irc::CompareServerPassword(password) == true)
	{
		is_registered_ = true;
		std::cout << GetNickname() << " now registered" << std::endl;
		return true;
	}
	return false;
}


/* =================			Operator Operations			================= */

void	User::SetMode(std::string channel_name, std::string mode) //invite only
{
	Channel* channel = Irc::GetChannel(channel_name);
	if (channel == NULL)
		return ;
	if (mode == "+i")
		channel->AddMode(MODE_INVITE_ONLY, this);
	else if (mode == "-i")
		channel->RemoveMode(MODE_INVITE_ONLY, this);
	else if (mode == "+t")
		channel->AddMode(MODE_TOPIC, this);
	else if (mode == "it")
		channel->RemoveMode(MODE_TOPIC, this);
	else
		WriteOutputBuff("mode: wrong input\n");
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

void	User::GetTopic(std::string channel_name)
{
	Channel* channel = Irc::GetChannel(channel_name);
	if (channel == NULL)
		return ;
	channel->GetTopic(this);
}

void	User::SetTopic(std::string channel_name, std::string new_topic)
{
	Channel* channel = Irc::GetChannel(channel_name);
	if (channel == NULL)
		return ;
	channel->SetTopic(new_topic, this);
}


/* =================				Getters				================= */

const std::string&	User::GetNickname() const { return nickname_; }
bool				User::IsRegistered() const { return is_registered_;}
bool				User::IsAuthenticated() const { return is_authenticated_;}


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

/* =================				Testfunctions				================= */

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