/* Invoke uppercase/lowercase filter to read commands */

#include "../apue.h"
#include "../standarderrorroutines.h"
#include <sys/wait.h>
#include <sys/select.h>

int
main(void)
{
	char line[MAXLINE];
	FILE *fpin;
	int fpind;
	fd_set  rset;
	
	if ((fpin = popen("./myuclc", "r")) == NULL)           
		err_sys("popen error");
	
	fpind = fileno(fpin);
	
	FD_ZERO(&rset);

	for ( ; ; ) {
		fputs("prompt> ", stdout);
		fflush(stdout);
		FD_SET(fpind, &rset);
		while (select(fpind + 1, &rset, NULL, NULL, NULL) < 0) {
			if (errno != EINTR) {
				err_sys("select error");
			}
			FD_SET(fpind, &rset);
		}
		if (FD_ISSET(fpind, &rset)) {
			if (fgets(line, MAXLINE, fpin) == NULL){ /* read from pipe */
				if(ferror(fpin)){
					err_sys("read from pipe error");
				}
				printf("write end of pipe closed");
				break;
			}
			if (fputs(line, stdout) == EOF)
				err_sys("fputs error to pipe");

		}
	}
	if (pclose(fpin) == -1)
		err_sys("pclose error");
	putchar('\n');
	exit(0); 
}
