#include "./Server.hpp"

Server::Server()
{

}

Server::~Server()
{
	
}
void	Server::PollEventHandler()
{
	// ResetPollFdFlags();
	pollfd*	poll_fds_array = &poll_fds_[0]; 
	int		poll_events_ready = poll(poll_fds_array, poll_fds_.size(), 0); //handle timeout
	if (poll_events_ready == -1)
		; //error here or in demultiplexer?
	// evaluate the returned events
	for (size_t idx = 0; idx < poll_fds_.size() && poll_events_ready; ++idx)
	{
		if (poll_fds_[idx].fd == server_fd_.fd)
		{
			// if (poll_fds_[idx].revents & POLLIN) // there is data to read
			// 	accept_client
			;
		}
		try{
		if (poll_fds_[idx].revents != 0)
		{
			--poll_events_ready;
			if (poll_fds_[idx].revents & POLLIN) // there is data to read
				conns_.at(poll_fds_[idx].fd).Recv();
			if (poll_fds_[idx].revents & POLLOUT)
				conns_.at(poll_fds_[idx].fd).Send();
			if (poll_fds_[idx].revents & POLLERR)
				conns_.at(poll_fds_[idx].fd).Error();
			if (poll_fds_[idx].revents & POLLHUP)
				conns_.at(poll_fds_[idx].fd).ClosedClient(); // recv should call close if pipe empty
			if (poll_fds_[idx].revents & POLLNVAL)
				; // fd not open
		}
		} catch (std::exception& e) { std::cerr << "exception caught: " << e.what() << '\n'; }
	}
}

void	Server::DeleteCollector(User* conn_identifier)
{
	conns_to_delete_.push_back(conn_identifier);
}

