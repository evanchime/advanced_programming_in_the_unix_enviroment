#include "apue.h" 
#include "standarderrorroutines.h"


int 
main(void)
{
	pid_t pid;
	
	if ((pid = fork()) < 0) {
		err_sys("fork error");
	}else if (pid == 0) { /* child */
		sleep(4); // parent go first
		if( getpgrp() == setsid())
			printf("child becomes a process group leader and child no longer has a controlling terminal");
		else
			printf("algorithm error");
		exit(0);
	}else
		exit(0); // parent terminates
}
