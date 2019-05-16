#include "exercise12.6.h"
#include <sys/time.h>

#define NTHR 10
#define UPPER 10
#define LOWER 1

struct arg{
	int thrdno;
        int thrdsecs;
};

void * 
thr_fn(void *arg) 
{ 
	struct arg *i = (struct arg*)arg;
	int j = i->thrdno;
	int k = i->thrdsecs;
	printf("before thread %d sleep\n", j);
        sleep(k);
        printf("After thread %d sleep\n", j);
	return ((void *)0);
}

int
main(void)
{
	int err, i; 
	pthread_t tid;

	srandom(time(0));

	for (i = 0; i < NTHR; i++) { 
		struct arg *arg;
		arg->thrdno = i + 1;
		arg->thrdsecs = (random() % (UPPER - LOWER + 1)) + LOWER;
		err = pthread_create(&tid, NULL, thr_fn, (void *)arg); 
		if (err != 0) 
			err_exit(err, "can’t create thread"); 
	}

	printf("before main thread sleep\n");
	sleep(3);
	printf("After main thread sleep\n");

	exit(0);
}
