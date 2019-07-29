#include "exercise12.6.h"
#include <sys/time.h>

#define NTHR 2
#define UPPER 4
#define LOWER 1

//pthread_barrier_t b;

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
	//pthread_barrier_wait(&b);
        printf("After thread %d sleep\n", j);
	//pthread_barrier_wait(&b);
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
		arg->thrdsecs =   (random() % (UPPER - LOWER + 1)) + LOWER;
		err = pthread_create(&tid, NULL, thr_fn, (void *)arg); 
		if (err != 0) 
			err_exit(err, "can’t create thread"); 
	}

	/*struct arg *arg;
        arg->thrdno = 1;
        arg->thrdsecs = 2;*/

	//pthread_barrier_init(&b, NULL, NTHR + 1);

	/*err = pthread_create(&tid, NULL, thr_fn, (void *)arg);
                if (err != 0)
                        err_exit(err, "can’t create thread");*/

	printf("before main thread sleep\n");
	sleep(4);
	//pthread_barrier_wait(&b);
	printf("After main thread sleep\n");
	
	//pthread_barrier_wait(&b);

	exit(0);
}
