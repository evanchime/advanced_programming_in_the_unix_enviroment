#include "../apue.h"
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "../tell_wait.h"
#include "../lock.h"
#include "../standarderrorroutines.h"

#define MY_PROCESSES 10

int
main(int argc, char *argv[])
{
	int fd, stat;
	pid_t pid1, pid2;

	if(argc != 2)
		err_quit("usage: ftw <starting-pathname>");

	if((fd = open(argv[1], O_RDWR | O_CREAT, FILE_MODE)) < 0)
		err_sys("open error");

	TELL_WAIT();

	if((pid1 = fork()) < 0){
	       err_sys("fork error");
	}else if(pid1 != 0){ /* parent */
		read_lock(fd, 0, SEEK_SET, 0);
		TELL_CHILD(pid1);
		while(waitpid(pid1, &stat, 0) < 0)
			if(errno != EINTR)
				exit(-1); /* error other than EINTR from waitpid() */
	}else{ /* child */
		WAIT_PARENT();  /* parent goes first */
		if((pid2 = fork()) < 0){
			err_sys("fork error");
		}else if(pid2 != 0){ /* parent */
			write_lock(fd, 0, SEEK_SET, 0);
			printf("from parent after write lock\n");
			while(waitpid(pid2, &stat, 0) < 0)
				if(errno != EINTR)
					exit(-1); /* error other than EINTR from waitpid() */

		}else{
			pid_t pid;
			for(int i = MY_PROCESSES; i > 0; --i){
				if((pid = fork()) < 0){
					err_sys("fork error");
				}else if(pid == 0){ /* child */
					if (execlp("./exercise14.1.helper1", "exercise14.1.helper1", argv[1], (char *)0) < 0)
						err_sys("execlp error");
				}
			}
		}			

	}
				
	exit(0);
}
