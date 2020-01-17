#include "apue.h"
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "lock.h"
#include "standarderrorroutines.h"


int
main(int argc, char *argv[])
{
	int fd;

        if(argc != 2)
                err_quit("usage: ftw <starting-pathname>");

        if((fd = open(argv[1], O_RDWR | O_CREAT, FILE_MODE)) < 0)
                err_sys("open error");

	read_lock(fd, 0, SEEK_SET, 0);
	printf("yes\n");
	alarm(300);
	exit(0);
}
