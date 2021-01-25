#include "apue.h"
#include "tell_wait.h"
#include "standarderrorroutines.h"
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>

#define NLOOPS      1000
#define SHM_SIZE    sizeof(long)    /* size of shared memory area */
#define SHM_MODE    0600    /* user read/write */

static int 
update(long *ptr){
	printf("%ld\n", *ptr);
	return((*ptr)++);   /* return value before increment */
}

int
main(void){
	int     fd, i, counter, shmid;
	void    *shmptr;
	pid_t   pid;

	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0)
		err_sys("shmget error");
	if ((shmptr = shmat(shmid, 0, 0)) == (void *)-1)
		err_sys("shmat error");

	TELL_WAIT();

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	}else if (pid > 0) {/* parent */
		for (i = 0; i < NLOOPS; i += 2) {
			if ((counter = update((long *)shmptr)) != i)
				err_quit("parent: expected %d, got %d", i, counter);
			TELL_CHILD(pid);
			WAIT_CHILD();
		}
	}else {/* child */
		for (i = 1; i < NLOOPS + 1; i += 2) {
			WAIT_PARENT();
			if ((counter = update((long *)shmptr)) != i)
				err_quit("child: expected %d, got %d", i, counter);
			TELL_PARENT(getppid());
		}
	}exit(0);
}	
