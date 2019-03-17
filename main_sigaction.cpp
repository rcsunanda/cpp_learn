/*
This program uses the newer and more recommended sigaction() function for signal handling
g++ --std=c++11 main_sigaction.cpp -o main_sigaction
*/

#include <iostream>
#include <string>
#include <vector>

#include <cstdlib>	//printf
#include <cstring>	//memset
#include <signal.h>
#include <unistd.h>	//sleep


static void signalHandler(int signalNumber, siginfo_t* info, void* context)
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

	std::cout << "Signal originated from process ID: " << info->si_pid << std::endl;
}

int main(int argc, char* argv[])
{
	
	//Register signal handler with kernel for multiple signals using sigaction()
	
	struct sigaction act;
	memset(&act, '\0', sizeof(act));
	
	act.sa_sigaction = signalHandler;
	act.sa_flags = SA_SIGINFO | SA_RESTART;
	sigfillset(&act.sa_mask);
	
	std::vector<int> signalVec = {SIGTERM, SIGHUP, SIGABRT, SIGCONT, SIGSEGV, SIGTRAP};
	for (int signalNumber: signalVec)
	{
		if (sigaction(signalNumber, &act, NULL) < 0)
		{
			std::cout << "Failed to register signal handler for signal number: " << signalNumber << std::endl;
		}
		else
		{
			std::cout << "Successfully registered signal handler for signal number: " << signalNumber << std::endl;
		}
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

	std::cout << "Program exiting below input while loop" << std::endl;

	
	while (true)
	{
		std::cout << "..." << std::flush;
		sleep(3);
	}
	

	return 0;
}
