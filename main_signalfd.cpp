/*
This program uses the newer signalfd() function for signal handling by monitoring file descriptors with select(), epoll() and similar methods
g++ --std=c++11 main_signalfd.cpp -o main_signalfd
*/

#include <iostream>
#include <vector>

#include <signal.h>
#include <sys/signalfd.h>

//For select()
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>	//read()

int main(int argc, char* argv[])
{
	std::cout << "Program started" << std::endl;
	//********************** create fd for monitoring signals

	sigset_t mask;	//signals to block
	int signalFD, fdmax;

	sigemptyset(&mask);

	std::vector<int> signalVec = {SIGTERM, SIGHUP, SIGABRT, SIGCONT, SIGSEGV, SIGTRAP};
	for (int signalNumber: signalVec)
	{
		sigaddset(&mask, signalNumber);	//add to mask
	}
	
	if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)	//block the signals in mask
	{
		perror ("sigprocmask()");
		return 1;
	}

	signalFD = signalfd(-1, &mask, 0);	//create FD. first argument = -1 means create new (else it modifies the given FD to monitor the given mask of signals)
	if (signalFD < 0)
	{
		perror ("signalfd");
		return 1;
	}

	//********************** monitoring signals with select()

	fd_set master, read_fds;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	FD_SET(signalFD, &master);
	fdmax = signalFD;

	while(true)
	{
		read_fds = master;

		select(fdmax+1, &read_fds, NULL, NULL, NULL);	//monitor FDs

		for (int fdi = 0; fdi <= fdmax; ++fdi)	//iterate all monitored FDs and check which one has data
		{
			if (FD_ISSET(fdi, &read_fds))
			{
				if (fdi == signalFD)
				{
					std::cout << "Signal received via FD: " << fdi << std::endl;

					struct signalfd_siginfo info;
					ssize_t bytes = read(fdi, &info, sizeof (info));
					
					if (bytes != sizeof(info))
					{
						std::cout << "Error reading from FD" << std::endl;
					}

					//information about the received signal
					unsigned sig = info.ssi_signo;
					unsigned user = info.ssi_uid;
					unsigned pid = info.ssi_pid;

					std::cout << "Signal no: " << sig << ", User ID: " << user << ", PID: " << pid << std::endl;
				}
			}
		}
	}
	return 0;
}
