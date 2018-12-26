/*
	*Recall the typical arrangement of memory in Figure7.6. 
	*Because the stack frames corresponding to each function call areusually stored in the stack, 
	*and because after a vfork the child runs in the address space of the parent, 
	*what happens if the call to vfork is from a function other than main and the child does a return from this function after the vfork?
	*Write a test program to verify this, and draw a picture of what’s happening.
*/

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

