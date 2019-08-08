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
	/*int fd;
	if ((fd = open(argv[1], O_RDWR|O_CREAT|O_TRUNC, FILE_MODE)) < 0) 
		err_sys("open error");*/
	//fprintf(fp, "before daemon");
	daemonize(argv[0]); //daemonize
	/*FILE *fp;
        if((fp = fopen(argv[1], "w+") == NULL))
                err_sys("fopen failed");*/
	//fprintf(fp, "in daemon now");
	if((name = getlogin()) == NULL)
		fprintf(fp, "can't daemonize");
	else
		fprintf(fp, "login name: %s", name);

	exit(0);
}
