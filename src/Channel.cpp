#include <Channel.hpp>
#include <MsgGeneration.hpp>
// #include <Server.hpp>

Channel::Channel(std::string name) : name_(name), topic_(""),  mode_flags_(0), gets_deleted(false)
{

}

Channel::~Channel()
{
}

bool	Channel::RegisterUser(User* user)
{
	try{
	if (user == NULL)
		return false;
	if (IsUserRegistered(user))
		return true;
	if ((mode_flags_ & MODE_INVITE_ONLY) && !IsUserInvited(user))
		return false; //not invited error
	std::vector<User*>::iterator it = registered_users_.begin();
	for(; it != registered_users_.end(); ++it)
	{
		if (*it.base() == user)
			return true; //user already registered
	}
	registered_users_.push_back(user);
	if (registered_users_.size() == 1)
		operator_ = user;
	} catch (std::exception& e) { std::cerr << "exception regi: " << e.what() << '\n'; }
	std::cout << user->GetNickname() << " joined channel: " << GetName() << std::endl;
	return true;
}

bool	Channel::DeregisterUser(User* user)
{
	std::vector<User*>::iterator it = registered_users_.begin();

	for(; it != registered_users_.end(); ++it)
	{
		if (*it.base() == user)
			break ;
	}
	if (it == registered_users_.end())
		return false;
	registered_users_.erase(it);
	if (user == operator_ && !registered_users_.empty())	//set new operator if user was operator
	{
		// only if no one is operator anymore
		operator_ = registered_users_.at(0); //sets new operator
		registered_users_.at(0)->WriteOutputBuff("you are now operator\r\n");
	}
	if (registered_users_.empty())
	{
			std::cout << "channel set to be deleted: "<< GetName() << std::endl;
		gets_deleted = true;
	}
	else
		gets_deleted = false;
	std::cout << user->GetNickname() << " left channeln" << std::endl;
	return true;
}

// broadcasts given msg to any user registered on the channel
void	Channel::BroadcastMsg(User& user, std::string msg)
{
	for(std::vector<User*>::iterator it = registered_users_.begin(); it != registered_users_.end(); ++it)
	{
		(*it)->WriteOutputBuff(gen_privmsg(user.GetNickname(), name_, msg));
	}
}

// returns channel name
std::string	Channel::GetName()
{
	return name_;
}

// sets flag for channel mode
void	Channel::AddMode(size_t flag, User* commanding_user)
{
	if (IsOperator(commanding_user) == false)
		return ;
	mode_flags_ |= flag;
	if (flag == MODE_INVITE_ONLY)
		std::cout << "invite only mode" << std::endl;
	if (flag == MODE_TOPIC)
		std::cout << "topic mode" << std::endl;
}

void	Channel::RemoveMode(size_t flag, User* commanding_user)
{
	if (IsOperator(commanding_user) == false)
		return ;
	mode_flags_ ^= flag;
	if (flag == MODE_INVITE_ONLY)
		std::cout << "NOT invite only mode" << std::endl;
	if (flag == MODE_TOPIC)
		std::cout << "NOT topic mode" << std::endl;
}

bool	Channel::InviteUser(User* new_user, User* commanding_user)
{
	if (new_user == NULL ||  IsOperator(commanding_user) == false)
		return false;
	invited_users_.push_back(new_user);
	return true;
}

bool	Channel::KickUser(User* kick_user, User* commanding_user)
{
	std::cout << "kick: " << kick_user << "from: " << commanding_user << std::endl;
	if (kick_user == NULL || IsOperator(commanding_user) == false)
		return false;
	DeregisterUser(kick_user);
	// kick_user->WriteOutputBuff("you got kicked\n");
	kick_user->WriteOutputBuff(gen_kick(commanding_user->GetNickname(), kick_user->GetNickname(), name_, "we dont need a reason"));
	return true;
}

void	Channel::SetTopic(std::string& new_topic, User* commanding_user)
{
	if (!(mode_flags_ & MODE_TOPIC))
		commanding_user->WriteOutputBuff("channel is in no topic mode\n");
	else if (IsOperator(commanding_user) == true) {
		// commanding_user->WriteOutputBuff("changed topic to:" + new_topic + "\n");
		topic_ = new_topic;
		for(std::vector<User*>::iterator it = registered_users_.begin(); it != registered_users_.end(); ++it)
		{
			(*it)->WriteOutputBuff(gen_set_topic(commanding_user->GetNickname(), name_, new_topic));
		}
	}
	else
		commanding_user->WriteOutputBuff("you are not operator\r\n");
}

void	Channel::GetTopic(User* commanding_user)
{
	if (!(mode_flags_ & MODE_TOPIC))
		commanding_user->WriteOutputBuff("channel is in no topic mode\n");
	else {
		commanding_user->WriteOutputBuff(gen_get_topic(name_, topic_));
	}
		// commanding_user->WriteOutputBuff(topic_ + "\n");
}


// returns true if a user is in the invited vector
bool	Channel::IsUserInvited(User* user)
{
	for(std::vector<User*>::iterator it = invited_users_.begin(); it != invited_users_.end(); ++it)
	{
		if (*it == user)
			return true;
	}
	return false;
}

bool	Channel::IsUserRegistered(User* user)
{
	for(std::vector<User*>::iterator it = registered_users_.begin(); it != registered_users_.end(); ++it)
	{
		if (*it == user)
		{
			// user->WriteOutputBuff(user->GetNickname() + "already in channel" + name_ + "\n");
			return true;
		}
	}
	return false;
}

bool	Channel::IsOperator(User* user)
{
	if (user != operator_)
	{
		user->WriteOutputBuff("you are not operator\r\n");
		return false;
	}
	return true;
}


