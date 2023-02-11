#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

// Output to stderr, contains file, function and line information
#define ERROR(msg) std::cerr << RED << __FILE__ << ": " << __func__ << ": " << __LINE__ << ": " << (msg) << RESET << std::endl;

// Print on stdout, contains file, function and line information
#define PRINTLN(msg) std::cout << CYAN << __FILE__ << ": " << __func__ << ": " << __LINE__ << ": " << (msg) << RESET << std::endl;

#endif
