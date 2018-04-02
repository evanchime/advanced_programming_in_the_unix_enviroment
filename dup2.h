#include "apue.h" 
#include "standardErrorRoutines.h"
#include <errno.h> 
#include <limits.h>
#include <unistd.h> 
#include "open_max.h"

int mydup2(int oldfd, int newfd){
    /*
    *Checks if the newfd argument is within the valid range
    *open_max gets the value of the total number of file descriptors
    */
    if (newfd >= open_max() || newfd < 0) 
		return -1;

    if (lseek(oldfd, 0, SEEK_CUR) < 0 && errno == EBADF) {//Determines if oldfd is a valid file descriptor.
		printf("file descriptor: %d is not valid\n", oldfd);
		return -1;
	}

    if (oldfd == newfd)// if the fds equal, return newfd
		return newfd;
    
    close(newfd); //Close newfd if open

    int fd[newfd]; //Array of fds != newfd
    int i = 0; //loop variable

    while((fd[i] = dup(oldfd)) >= 0){//Duplicate oldfd, until oldfd == newfd
        if(fd[i] == newfd){
            while (--i >= 0)//Close opened fds
			    close(fd[i]);
            return newfd;
        }
        i++; 
    }

    printf("dup: %s\n", strerror(errno));//Handle dup error
    while (--i >= 0)//Close opened fds
        close(fd[i]);
    return -1;
}