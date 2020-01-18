#include "../apue.h"
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include "../lock.h"
#include "../standarderrorroutines.h"


int
main(int argc, char *argv[])
{
        int fd, stat;
	//pid_t pid;

        if(argc != 2)
                err_quit("usage: ftw <starting-pathname>");

        if((fd = open(argv[1], O_RDWR | O_CREAT, FILE_MODE)) < 0)
                err_sys("open error");

        write_lock(fd, 0, SEEK_SET, 0);
        printf("from parent after write lock\n");
        
	/*pid = argv[2];
	while(waitpid(pid, &stat, 0) < 0)
                if(errno != EINTR)
			exit(-1);  //error other than EINTR from waitpid()*/

        exit(0);
}
