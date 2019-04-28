#include "apue.h" 
#include "standarderrorroutines.h"
#include <fcntl.h>
#include <sys/resource.h>

#define BUFFSIZE 100

static void 
sig_xfsz(int signo) { 
	printf("SIGXFSZ caught");
	return;
}

Sigfunc *
signal_intr(int signo, Sigfunc *func)
{
        struct sigaction act, oact;

        act.sa_handler = func;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
        if (sigaction(signo, &act, &oact) < 0)
                return(SIG_ERR);
        return(oact.sa_handler);
}

int 
main(int argc, char *argv[]) 
{ 
	if(signal_intr(SIGXFSZ, sig_xfsz) == SIG_ERR)
		err_sys("signal(SIGXFSZ) error");

	struct rlimit limit;
	
	if (getrlimit(RLIMIT_FSIZE, &limit) < 0) 
		err_sys("getrlimit error");
	
	limit.rlim_cur = 1024;

	if (setrlimit(RLIMIT_FSIZE, &limit) < 0) 
		err_sys("setrlimit error");

	int n, fd, fd2; 
        char buf[BUFFSIZE];
	
	if (argc != 3) 
		err_quit("usage: a.out <pathname>"); 
       
        if ((fd = open(argv[1], O_RDONLY )) < 0) 
	       err_sys("open error");

	if ((fd2 = open(argv[2], O_RDWR)) < 0)
               err_sys("open error");
	
	while ((n = read(fd, buf, BUFFSIZE)) > 0){ 
		if (write(fd2, buf, n) != n){
		        printf("%d\n", n);	
			err_sys("write error");
		}
	}	
	if (n < 0) 
		err_sys("read error"); 
	
	exit(0);
}

