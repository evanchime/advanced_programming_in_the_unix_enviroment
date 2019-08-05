#include "standarderrorroutines.h"
#include "daemonize.h"

int
main(int argc, char *argv[]){
	if (argc != 2) 
		err_quit("usage: ftw <starting-pathname>");
	char *name;
	FILE *fp;
        if((fp = fopen(argv[1], "w+") == NULL))
	        err_sys("fopen failed");
	daemonize(argv[0]); //daemonize
	if((name = getlogin()) == NULL)
		fprintf(fp, "can't daemonize");
	else
		fprintf(fp, "login name: %s", name);

	exit(0);
}
