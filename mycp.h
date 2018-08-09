/*
	*Write a utility like cp(1) that copies a file containing holes, 
	*without writing the bytes of 0 to the output
*/

#ifndef _MYCP_H 
#define _MYCP_H

#include "apue.h"
#include <fcntl.h>
#include "standardErrorRoutines.h"

#define RWXRWXRWX (S_IRWXU|S_IRWXG|S_IRWXO)

int
mycp(const char *fromPath, const char *toPath)  /* copy input path to output path */
{
	umask(0);
	//struct stat statbuf;
        //off_t fromPath_size; /* size in bytes, for copyFrom path */
	char c;
	int n, fromPathFd, toPathFd;

	/*if (stat("fromPath", &statbuf) < 0) 
		err_sys("stat error for fromPath");*/
	//fromPath_size = statbuf.st_size;
	
	if((fromPathFd = open(fromPath, O_RDONLY)) < 0)
		err_sys("fromPath open error");
 
	if((toPathFd = open(toPath, O_RDWR | O_CREAT | O_TRUNC, RWXRWXRWX)) < 0)
		err_sys("toPath open error"); 

	while ((n = read(fromPathFd, &c, 1)) > 0){
        	if(c == '\0')
			continue;
		if (write(toPathFd, &c, n) != n) 
			err_sys("write error");
	}

	if (n < 0) 
		err_sys("read error");
	
    	return 0;
}

#endif /* _MYCP_H */
