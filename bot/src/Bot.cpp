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
	servaddr.sin_addr.s_addr = INADDR_ANY; //automatically sets to 127.0.0.1

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
	close(socket_.fd);
}

void	Bot::InitBot(std::string server_password)
{
	(void)server_password;
	char data[11] = "PASS ewf\r\n";
	char nick[11] = "NICK bot\r\n";
	char user[11] = "USER bot\r\n";
	int bytesRead = send(socket_.fd, static_cast<char*>(data), 10, 0);
	bytesRead = send(socket_.fd, static_cast<char*>(nick), 10, 0);
	bytesRead = send(socket_.fd, static_cast<char*>(user), 10, 0);
}

void	Bot::Recv()
{
	char		buff[512];
	ssize_t		c_received;


	c_received = recv(socket_.fd, &buff, 512, 0);
	if (c_received == 0)
	{
		input_buff_.clear();
		exit(0);
	}
	else 
	{
		for (int i = 0; i < c_received; ++i)
			input_buff_.push_back(buff[i]);
		for (std::vector<char>::iterator it = input_buff_.begin(); it != input_buff_.end(); ++it) //test print function
			std::cout << *it;
		std::cout << std::endl;
	}
	if (input_buff_.size() == 0)
		return ;
	for (std::vector<char>::iterator it = input_buff_.begin() + 1; it != input_buff_.end(); ++it)
	{
		if (*it == ' ' || *it == '!')
			break ;
		output_buff_.push_back(*it);
	}
	// HERE OUTPUT BUFF
	input_buff_.clear();
}

void	Bot::SendTime()
{
	if (output_buff_.empty())
		return ;
	std::string str = "PRIVMSG ";
	output_buff_.insert(output_buff_.begin(), str.begin(), str.end());
	output_buff_.push_back(' ');
	output_buff_.push_back(':');
	GetTime();
	int bytesRead = send(socket_.fd, static_cast<char*>(output_buff_.data()), output_buff_.size(), 0);
	if( bytesRead >= 0)
		output_buff_.erase(output_buff_.begin(), output_buff_.begin()+ bytesRead);
	else
		exit(1);

}

void	Bot::GetTime()
{
	time_t		now = time(0);
	struct tm 	tstruct = *localtime(&now);;
	char		buf[80];
	std::string	str;

	strftime(buf, sizeof(buf), "%Y-%m-%d.%X\r\n", &tstruct);
	str = static_cast<std::string>(buf);
	std::replace(str.begin(), str.end(), ':', '.');
	output_buff_.insert(output_buff_.end(), str.begin(), str.end());
}

