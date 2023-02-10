#include <Parser.hpp>

#include <iostream>

int	main(int argc, char** argv) {
	if (argc > 1) {
		Parser	pars(argv[1]);

		message_t	message = pars.parse_message();
		std::cout << message.prefix.name << "   " << message.prefix.user << "   " << message.prefix.host << std::endl;
		std::cout << message.command << std::endl;
		for (std::vector<std::string>::iterator it = message.params.begin(); it != message.params.end(); ++it) {
			std::cout << *it << std::endl;
		}
	}
}
