/*
	*Write a program that calls the daemonize function in Figure13.1. 
	*After calling this function, call getlogin (Section 8.15) to see whether the process has a login name now that it has become a daemon. 
	*Print the results to a ﬁle.
*/

/*
	* compile as sudoer
        * run as sudoer	
*/

#include "standarderrorroutines.h"
#include "daemonize.h"

int
main(int argc, char *argv[]){
	if (argc != 2) 
		err_quit("usage: ftw <starting-pathname>");
	char *name;
	daemonize(argv[0]); //daemonize
	FILE *fp;
        if((fp = fopen(argv[1], "w+")) == NULL)
                err_sys("fopen failed");
	if((name = getlogin()) == NULL)
		fprintf(fp, "can't get login name");
	else
		fprintf(fp, "login name: %s", name);

	exit(0);
}
