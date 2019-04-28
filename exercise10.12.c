#include "apue.h"
#include "standarderrorroutines.h"

#define BUFSIZE 1000000000

static void 
sig_alrm(int signo) { 
	printf("SIGALRM caught\n");
	return; 
}

char buf[BUFSIZE];

int
main(int argc, char *argv[])
{

	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		err_sys("signal(SIGALRM) error");

	if (argc != 2)
        	err_quit("usage: a.out <pathname>");

	FILE *fp; 
	
	if ((fp = fopen(argv[1], "w+")) == NULL) 
		err_sys("fopen failed");

	alarm(1);

	if (fwrite(buf, sizeof(char), BUFSIZE, fp) != BUFSIZE) 
		err_sys("fwrite error");
	
	exit(0);
}
