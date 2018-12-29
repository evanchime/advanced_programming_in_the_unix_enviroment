/*
	* Write a program that creates a zombie, and then call system to execute the ps(1) command to verify that the process is a zombie.
*/

#include "apue.h" 
#include "standarderrorroutines.h"


int
main(void)
{
	pid_t pid;
	int status;

	if ((pid = fork()) < 0) { 
		err_sys("fork error"); 
	}else if (pid == 0) { /* child */ 
		_exit(0); /* child terminates */	
	}
	
	/* parent continues here */
	sleep(3);
	
	if((status = system("ps")) == -1 || status == 127)
		err_sys("system error");

	exit(0);
}
