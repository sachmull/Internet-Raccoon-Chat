#include "./Bot.hpp"

Bot::Bot(int port)
{
	int	count = 0;
	// int	error;
	struct	sockaddr_in servaddr;

	socket_.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_.fd == -1)
		throw std::runtime_error("socket") ;

	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = INADDR_ANY; //set to local inet addr?

	// connect the client socket to server socket
	while (connect(socket_.fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		if (count >= 5)
			exit(0);
		sleep(3);
		count++;
	}
}

Bot::~Bot()
{
	//maybe close port
}


void	Bot::Recv()
{
	char		buff[512];
	ssize_t		c_received;


	c_received = recv(socket_.fd, &buff, 512, 0);
	if (c_received == -1)
	{
		input_buff_.clear();
		//quit bot?
	}
	else 
	{
		for (int i = 0; i < c_received; ++i)
			input_buff_.push_back(buff[i]);
		for (std::vector<char>::iterator it = input_buff_.begin(); it != input_buff_.end(); ++it) //test print function
			std::cout << *it;
		std::cout << std::endl;
	}

	// HERE OUTPUT BUFF
	input_buff_.clear();
}

void	Bot::SendTime()
{
	if (output_buff_.empty())
		return ;
	int bytesRead = send(socket_.fd, static_cast<char*>(output_buff_.data()), output_buff_.size(), 0);
	if( bytesRead > 0)
		output_buff_.erase(output_buff_.begin(), output_buff_.begin()+ bytesRead);
	else
		;

}
