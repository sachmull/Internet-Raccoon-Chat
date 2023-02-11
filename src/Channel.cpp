#include <Channel.hpp>
// #include <Server.hpp>

Channel::Channel(std::string name) : name_(name)
{

}

Channel::~Channel()
{
	
}

void	Channel::RegisterUser(User* user)
{
	registered_users_.push_back(user);
	if (registered_users_.size() == 1)
		user->SetOperator(true);
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
	{
		registered_users_.erase(it);
		user->SetOperator(false);
		//set new operator if user was operator
	}
	if (registered_users_.size() == 0)
		; //delete channel
}

void	Channel::BroadcastMsg(std::vector<char> msg)
{
	for(std::vector<User*>::iterator it = registered_users_.begin(); it != registered_users_.end(); ++it)
	{
		(*it)->WriteOutputBuff(msg);
	}
}