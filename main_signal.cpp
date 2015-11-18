/*
This program uses the classic signal() function for signal handling
g++ --std=c++11 main_signal.cpp -o main_signal
*/

#include <iostream>
#include <string>
#include <vector>

#include <cstdlib>
#include <signal.h>


void signalHandler(int signalNumber)
{
	std::cout << "Received signal number: " << signalNumber << std::endl;
	
	if (signalNumber == SIGTERM)
	{
		std::cout << "Received signal is SIGTERM (Terminate)" << std::endl;
	}
	else if (signalNumber == SIGHUP)
	{
		std::cout << "Received signal is SIGHUP (Hangup)" << std::endl;
	}
	else if (signalNumber == SIGABRT)
	{
		std::cout << "Received signal is SIGABRT (Abort)" << std::endl;
	}
	else if (signalNumber == SIGSTOP)
	{
		std::cout << "Received signal is SIGSTOP (SIGSTOP)" << std::endl;
	}
	else if (signalNumber == SIGCONT)
	{
		std::cout << "Received signal is SIGCONT (SIGCONT)" << std::endl;
	}
	else if (signalNumber == SIGSEGV)
	{
		std::cout << "Received signal is SIGSEGV (SIGSEGV)" << std::endl;
	}
	else if (signalNumber == SIGTRAP)
	{
		std::cout << "Received signal is SIGTRAP (SIGTRAP)" << std::endl;
	}
	else if (signalNumber == SIGKILL)	//This should not execute as SIGKILL cannot be caught
	{
		std::cout << "Received signal is SIGKILL (SIGKILL)" << std::endl;
	}
}

int main(int argc, char* argv[])
{
	
	//Register signal handler with kernel for multiple signals
	std::vector<int> signalVec = {SIGTERM, SIGHUP, SIGABRT, SIGSTOP, SIGCONT, SIGSEGV, SIGTRAP, SIGKILL};
	for (int signalNumber: signalVec)
	{
		if (signal(signalNumber, signalHandler) == SIG_ERR)
		{
			std::cout << "Failed to register signal handler for signal number: " << signalNumber << std::endl;
		}
		else
		{
			std::cout << "Successfully registered signal handler for signal number: " << signalNumber << std::endl;
		}
	}
	
	//Ignore (block) SIGINT signal
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
	{
		std::cout << "Failed to ignore SIGINT" << std::endl;
	}
	else
	{
		std::cout << "Successfully ignored SIGINT" << std::endl;
	}

	//Default handler for SIGHUP
	if (signal(SIGHUP, SIG_DFL) == SIG_ERR)
	{
		std::cout << "Failed to set default handler for SIGHUP" << std::endl;
	}
	else
	{
		std::cout << "Successfully set default handler for SIGHUP" << std::endl;
	}

	std::cout << "Starting signal handling program (type 'abort' to exit) " << std::endl;

	std::string line;

	while (std::getline(std::cin, line))
	{
		std::cout << "User input line: " << line << std::endl;

		if (line == "abort")
		{
			std::cout << "Aborting" << std::endl;
			abort();
		}
	}

	return 0;
}
