#include <Channel.hpp>
// #include <Server.hpp>

Channel::Channel(std::string name) : name_(name), mode_flags_(0)
{

}

Channel::~Channel()
{
	
}

bool	Channel::AddInvitedUser(User* user)
{
	if (user == NULL)
		return false;
	invited_users_.push_back(user);
	return true;
}

bool	Channel::RegisterUser(User* user)
{
	try{
	if (user == NULL)
		return false;
	if ((mode_flags_ && MODE_INVITE_ONLY) && !IsUserInvited(user))
		return false; //not invited error
	std::vector<User*>::iterator it = registered_users_.begin();
	for(; it != registered_users_.end(); ++it)
	{
		if (*it.base() == user)
			return true; //user already registered
	}
	registered_users_.push_back(user);
	if (registered_users_.size() == 1)
		user->SetOperator(true);
	} catch (std::exception& e) { std::cerr << "exception regi: " << e.what() << '\n'; }
	return true;
}

void	Channel::DeregisterUser(User* user)
{
	std::vector<User*>::iterator it = registered_users_.begin();

	for(; it != registered_users_.end(); ++it)
	{
		if (*it.base() == user)
			break ;
	}
	if (it != registered_users_.end())
	{
		registered_users_.erase(it);
		//set new operator if user was operator
	}
	if (registered_users_.size() == 0)
		return ; //delete channel
	else if (user->IsOperator())
	{
		user->SetOperator(false);
		std::string tmp = "pw";
		registered_users_.at(0)->GetOperator(tmp); //sets new operator
		registered_users_.at(0)->WriteOutputBuff("you are now operator");
	}
}

// broadcasts given msg to any user registered on the channel
void	Channel::BroadcastMsg(std::vector<char> msg)
{
	for(std::vector<User*>::iterator it = registered_users_.begin(); it != registered_users_.end(); ++it)
	{
		(*it)->WriteOutputBuff(msg);
	}
}

// returns channel name
std::string	Channel::GetName()
{
	return name_;
}

// sets flag for channel mode
void	Channel::SetMode(size_t flag)
{
	mode_flags_ += flag;
	if (mode_flags_ && MODE_INVITE_ONLY)
		std::cout << "invite only mode" << std::endl;
	else
		std::cout << "NOT invite only mode" << std::endl;
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

bool	Channel::KickUser(User* user)
{
	if (user == NULL)
		return false;
	user->DisconnectFromChannel();
	return true;
}
