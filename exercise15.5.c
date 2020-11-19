/* 
	* In  the  program  in  Figure15.18,  use  the  standardI/O  library  
	* for  reading  and  writing  the pipes instead of read and write
*/ 

#include "apue.h"
#include "standarderrorroutines.h"

static void sig_pipe(int);      /* our signal handler */

int
main(void)
{
	int     n, fd1[2], fd2[2];
	FILE *fd1pout, *fd2pin;
	pid_t   pid;
	char    line[MAXLINE];
	
	if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
		err_sys("signal error");
	
	if (pipe(fd1) < 0 || pipe(fd2) < 0)
		err_sys("pipe error");
	
	if ((pid = fork()) < 0) {
		err_sys("fork error");
	}else if (pid > 0) {/* parent */
		close(fd1[0]);
		close(fd2[1]);

		if((fd1pout = fdopen(fd1[1], "w")) == NULL)
                        err_sys("fdopen error");
		if((fd2pin = fdopen(fd2[0], "r")) == NULL)
                        err_sys("fdopen error");

		if (setvbuf(fd2pin, NULL, _IOLBF, 0) != 0)
                        err_sys("setvbuf error");
                if (setvbuf(fd1pout, NULL, _IOLBF, 0) != 0)
                        err_sys("setvbuf error");

		while (fgets(line, MAXLINE, stdin) != NULL) {
			if(fputs(line, fd1pout) == EOF)
				err_sys("output error");
			if(fgets(line, MAXLINE, fd2pin) == NULL){
				if(ferror(fd2pin)){
					err_sys("error reading from pipe");
				}else{
					err_msg("child closed pipe");
				}
			}

			if (fputs(line, stdout) == EOF)
				err_sys("fputs error");
		}if (ferror(stdin))
			err_sys("fgets error on stdin");
		exit(0);
	}else {/* child */
		close(fd1[1]);
		close(fd2[0]);
		if (fd1[0] != STDIN_FILENO) {
			if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
				err_sys("dup2 error to stdin");
			close(fd1[0]);
		}if (fd2[1] != STDOUT_FILENO) {
			if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
				err_sys("dup2 error to stdout");
			close(fd2[1]);
		}if (execl("./add2", "add2", (char *)0) < 0)
		err_sys("execl error");
	}
	exit(0);
}

static void
sig_pipe(int signo){
	
	printf("SIGPIPE caught\n");
	exit(1);
}
