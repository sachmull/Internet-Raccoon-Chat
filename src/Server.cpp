#include <Server.hpp>
std::vector<pollfd>			Server::poll_fds_;
Irc							Server::irc_;
Server::Socket				Server::socket_;

Server::Server(int port, int queue_length, std::string password)
{
	int	error;

	// set server password
	irc_.SetPassword(password);

	// create the server socket
	socket_.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_.fd == -1)
		throw std::runtime_error("socket") ;


	// make the server socket unblocking
	int status = fcntl(socket_.fd, F_GETFL, 0);
	if (status == -1)
		throw std::runtime_error("fcntl") ;
	error = fcntl(socket_.fd, F_SETFL, status | O_NONBLOCK);
	if (error == -1)
		throw std::runtime_error("fcntl") ;

	// define the server socket
	socket_.address.sin_family = AF_INET;
	socket_.address.sin_port = htons(port);
	socket_.address.sin_addr.s_addr = INADDR_ANY;

	// bind the socket to our specified IP and port
	error = bind(socket_.fd, (struct sockaddr*) &socket_.address, sizeof(socket_.address));
	if (error == -1)
		throw std::runtime_error("bind") ;
	error = listen(socket_.fd, queue_length);
	if (error == -1)
		throw std::runtime_error("listn") ;
	poll_fds_.push_back((struct pollfd){.fd = socket_.fd, .events = POLLEVENTS, .revents = 0});
}

Server::~Server()
{
	
}

/* =================				Server Actions				================= */

void	Server::PollEventHandler()
{
	ResetPollFdFlags();
	irc_.GetReady();
	pollfd*	poll_fds_array = &poll_fds_[0];
	int		poll_events_ready = poll(poll_fds_array, poll_fds_.size(), 0); //handle timeout
	if (poll_events_ready == -1)
		return ;
	for (size_t idx = 0; idx < poll_fds_.size() && poll_events_ready; ++idx) //delete idx < poll_fds_.size()?
	{
		try{
		if (poll_fds_.at(idx).fd == socket_.fd)
		{
			if (poll_fds_.at(idx).revents & POLLIN) // there is data to read
			{
				--poll_events_ready;
				Server::Accept();
				std::cout << "new client accepted" << std::endl;
			}
		}
		else if (poll_fds_.at(idx).revents != 0)
		{
			--poll_events_ready;
			if (poll_fds_.at(idx).revents & POLLIN) // there is data to read
				irc_.Recv(poll_fds_.at(idx).fd);
			if (poll_fds_.at(idx).revents & POLLOUT)
				irc_.Send(poll_fds_.at(idx).fd);
			if (poll_fds_.at(idx).revents & POLLERR)
				irc_.Error(poll_fds_.at(idx).fd);
			if (poll_fds_.at(idx).revents & POLLHUP)
				irc_.ClosedClient(poll_fds_.at(idx).fd); // recv should call close if pipe empty
			if (poll_fds_.at(idx).revents & POLLNVAL)
				irc_.ClosedConnection(poll_fds_.at(idx).fd); // fd not open
		}
		} catch (std::exception& e) { std::cerr << "irc: " << e.what() << '\n'; }
	}
}

void	Server::ErasePollFd(int fd)
{
	for(std::vector<pollfd>::iterator it = poll_fds_.begin(); it != poll_fds_.end(); ++it)
	{
		if(it->fd == fd)
		{
			close(it->fd);
			poll_fds_.erase(it);
			break ;
		}
	}
}

/* =================				Private Helpers				================= */

void	Server::ResetPollFdFlags()
{
	for(std::vector<pollfd>::iterator it = poll_fds_.begin(); it != poll_fds_.end(); ++it)
	{
		it->events = POLLEVENTS;
		it->revents = 0;
	}
}

void	Server::Accept()
{
	int				conn_fd;
	struct sockaddr	their_addr;
	socklen_t		addr_size;


	addr_size = sizeof(their_addr);
	conn_fd = accept(socket_.fd, &their_addr, &addr_size);

	// error handling
	if (conn_fd == -1)
		return ;

	AddConnection((struct pollfd){.fd = conn_fd, .events = POLLEVENTS, .revents = 0}, their_addr);
}

void Server::AddConnection(struct pollfd poll_fd, struct sockaddr addr)
{
	(void)addr;
	poll_fds_.push_back(poll_fd);
	Irc::AddUser(poll_fds_.back());
}
