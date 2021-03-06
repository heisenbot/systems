/*	Angshuman Ghosh	2017CS01

Program to handle signals and block other signals while the current handler is executing. Parent process generates the kill signals,
child process handles the signals.


Note : SIGTERM will not be unblocked as SIGQUIT is present and the program exits before SIGTERM is unblocked

*/




#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>



struct sigaction sigAct;	// declare signal structure
sigset_t block;			// declare signal set




void term(int sig){		// signal handler

	switch(sig){

		case SIGQUIT:
			printf("\nCaught SIGQUIT signal .... exiting\n");
			exit(0);
			break;
		case SIGINT:
			printf("\nCaught SIGINT signal\n");
			sleep(10);
			printf("Exiting SIGINT routine\n");
			break;
		case SIGHUP:
			printf("\nCaught SIGHUP signal\n");
			sleep(10);
			printf("Exiting SIGHUP routine\n");
			break;
		case SIGTERM:
			printf("\nCaught SIGTERM signal\n");
			sleep(5);
			printf("Exiting SIGTERM routine\n");
			break;

	}
	
}


int main(){

	int rv, ret;
	pid_t pid;

//============  sigaction structure initialization ==============//
	sigemptyset(&block);
	
	sigfillset(&block);	// fill the set with all signal types

	sigAct.sa_flags = 0;
	sigAct.sa_handler = term;
	sigAct.sa_mask = block;

//==============================================================//

	pid = fork();	// fork child

	switch(pid){

		case -1:
			printf("Child spawn unsuccessful\n");
			return 1;
		case 0:
			//	install the signal handlers //
			sigaction(SIGINT, &sigAct, NULL);
			sigaction(SIGHUP, &sigAct, NULL);
			sigaction(SIGTERM, &sigAct, NULL);
			sigaction(SIGQUIT, &sigAct, NULL);

			printf("Child started executing\n");

			while(1);	// wait for signals generated by parent

		default:
			sleep(1);
			
			kill(pid, SIGINT);
			printf("Generated SIGINT\n");
			
			sleep(1);
			
			ret=kill(pid, SIGHUP);
			printf("Generated SIGHUP\n");
			
			sleep(1);
			
			ret = kill(pid, SIGTERM);
			printf("Generated SIGTERM\n");
			
			sleep(1);
			
			ret=kill(pid, SIGQUIT);
			printf("Generated SIGQUIT\n");


	}

	wait(&rv);

	printf("Child returned exit code : %d\n", WEXITSTATUS(rv));

	return 0;
}       
