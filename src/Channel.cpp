#include "./Channel.hpp"

Channel::Channel()
{

}

Channel::~Channel()
{
	
}

void	Channel::RegisterUser(User* user)
{
	registered_users_.push_back(user);
}
void	Channel::DeregisterUser(User* user)
{
	std::vector<User*>::iterator it = registered_users_.begin();
	for(; it != registered_users_.end(); ++it)
	{
		if (*it == user)
			break ;
	}
	if (it != registered_users_.end())
		registered_users_.erase(it);
}

void	Channel::BroadcastMsg(std::vector<char> msg)
{
	for(std::vector<User*>::iterator it = registered_users_.begin(); it != registered_users_.end(); ++it)
	{
		(*it)->WriteOutputBuff(msg);
	}
}