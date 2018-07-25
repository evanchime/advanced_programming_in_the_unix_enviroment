/* 
	*The program creates a file: with a umask of 777
	*And access permission bits of RWRWRW - #defined below
	*Want to verify what happens
	*The expected effect depends on the filesystem
*/  
#include "apue.h"
#include <fcntl.h>
#include "standardErrorRoutines.h" 

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int 
main(void) 
{ 
	printf("%o\n",umask(0777));
  	if (creat("foo", RWRWRW) < 0) 
		err_sys("creat error for foo"); 
	printf("%o\n",umask(0)); 
	exit(0); 
}
