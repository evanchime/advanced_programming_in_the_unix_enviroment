/*
	* Redo the program in Figure 15.33 using the XSI semaphore functions 
	* from Section 15.8 toalternate between the parent and the child.
*/

#include "apue.h"
#include "standarderrorroutines.h"
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/mman.h>

#define NLOOPS      1000
#define NOPS        1 // no of operations
#define SIZE    sizeof(long)    /* size of shared memory area */

static int 
update(long *ptr){
	printf("%ld\n", *ptr);
	return((*ptr)++);   /* return value before increment */
}

union semun {
	int val;   /* for SETVAL */
	struct semid_ds  *buf;   /* for IPC_STAT and IPC_SET */
	unsigned short   *array; /* for GETALL and SETALL */
};

int
main(void){
	int     fd, i, counter, semid;
	pid_t   pid;
	union semun arg;
	struct sembuf semoparray[NOPS];
	void    *area;

	if ((fd = open("/dev/zero", O_RDWR)) < 0)
		err_sys("open error");
	if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
		err_sys("mmap error");
	close(fd);      /* can close /dev/zero now that it’s mapped */

	if ((semid = semget(IPC_PRIVATE, 1, 0666)) < 0)
		err_sys("semget error");

	arg.val = 2; // semaphore value
	semctl(semid, 0, SETVAL, arg );

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	}else if (pid > 0) {/* parent */
		
		struct sembuf semoparray1[NOPS];
		semoparray1[0].sem_num = 0;
		semoparray1[0].sem_op = -1;
		semoparray1[0].sem_flg = SEM_UNDO;

		for (i = 0; i < NLOOPS; i += 2) {
			semop(semid, semoparray1, 1);
			if ((counter = update((long *)area)) != i)
				err_quit("parent: expected %d, got %d", i, counter);
			arg.val = 0; // semaphore value
			semctl(semid, 0, SETVAL, arg );

		}
	}else {/* child */
		
		struct sembuf semoparray2[NOPS];
		semoparray2[0].sem_num = 0;
		semoparray2[0].sem_op = 0;
		semoparray2[0].sem_flg = SEM_UNDO;
		
		for (i = 1; i < NLOOPS + 1; i += 2) {
			semop(semid, semoparray2, 1);
			if ((counter = update((long *)area)) != i)
				err_quit("child: expected %d, got %d", i, counter);
			arg.val = 2; // semaphore value
                        semctl(semid, 0, SETVAL, arg );
		}
	}
	exit(0);
}	
