#include "apue.h" 
#include "standarderrorroutines.h"

int vfork_inside()
{
	pid_t pid;
	int x = 1;
	int y = 2;

	if ((pid = vfork()) < 0) { 
		err_sys("vfork error"); 
	}else if (pid == 0) { //child  
		return x; // return from function by child  
	       _exit(0); // child terminates 	
	}
	
	printf("parent\n");
	return y; // return from function by parent 
}

int 
main(void) 
{ 
	printf("before vfork\n");
	
	int z = vfork_inside();

	printf("after vfork, z = %d\n", z);
	
	exit(0); 
}

