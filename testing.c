#include "apue.h"
#include "standarderrorroutines.h"
#include <time.h>

typedef time_t TIME;		/* how time is actually stored */

#define TRUE	1
#define FALSE	0

#define MAX_TIMERS	10	/* number of timers */
#define VERY_LONG_TIME	(1  << ((sizeof(TIME) << 3) - 1)) - 1	/* longest time possible */
#define SECTONSEC 1000000000 /* seconds to nanoseconds */
#define NSECTOSEC(nsecs) nsecs/SECTONSEC /* nanoseconds to seconds */
#ifndef CLOCK_REALTIME 
#define CLOCK_REALTIME 0 
#define USECTONSEC 1000 /* microseconds to nanoseconds */ 
void clock_gettime(int id, struct timespec *tsp) { 
    struct timeval tv; 
    gettimeofday(&tv, NULL); 
    tsp->tv_sec = tv.tv_sec; 
    tsp->tv_nsec = tv.tv_usec * USECTONSEC; 
} 
#endif 

struct timer {
	int inuse;		/* TRUE if in use */
	TIME time;		/* relative time to wait */
	char *event;		/* set to TRUE at timeout */
} timers[MAX_TIMERS];		/* set of timers */

TIME time_now; /* time now */
struct timer *timer_next = NULL;/* timer we expect to run down next */
TIME time_timer_set;		/* time when physical timer was set */
sigset_t newmask, oldmask;      /* for signal masks */

TIME sectonsec(TIME); /* seconds to nanoseconds */
TIME time(TIME *); /* our version of time_t time(time_t *) with nanoseconds granularity */
void timers_init(void); /* initialize timers*/
void timer_undeclare(struct timer *); /* undeclare timers */
void timers_update(TIME);		/* subtract time from all timers, enabling any that run out along the way */
struct timer *timer_declare(unsigned int, char *);		/* time to wait in secs */
static void sig_alrm(int);     /* SIGALRM handler */
void disable_sigalrm_interrupts(void); /* disable SIGALRM */
void enable_sigalrm_interrupts(void); /* enable SIGALRM */

int 
main(int argc, char const *argv[])
{

	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		err_sys("can’t catch SIGALRM");

	timers_init();  

	timer_declare(1, "first timer\n");
	timer_declare(2, "second timer\n");
	timer_declare(3, "third timer\n");

	int i, j; 
	volatile int k; 
	/* 
		*	Tune these loops to run for more than 5 seconds 
		*	on whatever system this test program is run. 
	*/ 
	printf("\nsig_int starting\n"); 
	for (i = 0; i < 3000000; i++) 
		for (j = 0; j < 4000; j++) 
			k += i * j; 
	printf("sig_int finished\n");

	//printf("%ld", ((long)1  << ((sizeof(long) << 3) - 1)) - 1);
	//printf("%ld", sizeof(long));
	//printf("%ld", ((TIME)1 << (sizeof(TIME) << 3) - 1));
	//printf("%d", 1/*time(NULL)*/);
	return 0;
}

/* initialize timers*/
void
timers_init() {
	struct timer *t;

	for (t = timers; t < &timers[MAX_TIMERS]; t++)
		t->inuse = FALSE;
}

void
timer_undeclare(struct timer *t)
{
	disable_sigalrm_interrupts();
	if (!t->inuse) {
		enable_sigalrm_interrupts();
		return;
	}

	t->inuse = FALSE;

	/* check if we were waiting on this one */
	if (t == timer_next) {
        time_now = time(NULL);
		timers_update(time_now - time_timer_set);
		if (timer_next) {
			alarm(NSECTOSEC(timer_next->time));
			time_timer_set = time_now;
		}
	}
	enable_sigalrm_interrupts();
}

void disable_sigalrm_interrupts(void){
	sigemptyset(&newmask); 
	sigaddset(&newmask, SIGALRM);
	/* 
		*Block SIGALRM and save current signal mask. 
	*/ 
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) 
		err_sys("SIG_BLOCK error");
}

void enable_sigalrm_interrupts(void){
	/* 
		*Reset signal mask which unblocks SIGALRM. 
	*/ 
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) 
		err_sys("SIG_SETMASK error");
}

void
timers_update(TIME time)
{
	/*
		*timer_last is just a temporary.  It is a permanently non-schedulable
		*timer that will only show up when all the other timers have been scheduled.
	*/
	static struct timer timer_last = {
		FALSE			/* in use */,
		VERY_LONG_TIME		/* time */,
		NULL			/* event pointer */
	};

	struct timer *t;

	timer_next = &timer_last;

	for (t=timers;t<&timers[MAX_TIMERS];t++) {
		if (t->inuse) {
			if (time < t->time) { /* unexpired */
				t->time -= time;
				if (t->time < timer_next->time)
					timer_next = t;
			} else { /* expired */
				/* tell scheduler */
				//*t->event = TRUE;
				printf("%s", t->event);
				t->inuse = 0;	/* remove timer */
			}
		}
	}

	/* reset timer_next if no timers found */
	if (!timer_next->inuse) timer_next = 0;
}

struct timer *
timer_declare(unsigned int time_, char *event)		/* time to wait in secs */
{
	struct timer *t;

	disable_sigalrm_interrupts();

	if (time_ == 0) {
		timers_update(sectonsec(alarm(time_)));
		enable_sigalrm_interrupts();
		return 0;
	}
	
	for (t=timers;t<&timers[MAX_TIMERS];t++) {
		if (!t->inuse) 
        break;
	}

	/* out of timers? */
	if (t == &timers[MAX_TIMERS]) {
		enable_sigalrm_interrupts();
		return(0);
	}

	/* install new timer */
	t->event = event;
	t->time = sectonsec(time_);
	if (!timer_next) {
		/* no timers set at all, so this is shortest */
		time_timer_set = time(NULL);
		alarm(NSECTOSEC((timer_next = t)->time));
	} else if (time_ + (time_now = time(NULL)) < (timer_next->time + time_timer_set)) {
		/* new timer is shorter than current one, so */
		timers_update(time_now - time_timer_set);
		time_timer_set = time_now;
		alarm(NSECTOSEC((timer_next = t)->time)); 
	} else {
		/* new timer is longer, than current one */
	}
	t->inuse = TRUE;
	enable_sigalrm_interrupts();
	return(t);
}

static void 
sig_alrm(int signo) { 
	timers_update((time_now = time(NULL)) - time_timer_set);
	/* start physical timer for next shortest time if one exists */
	if (timer_next) {
		time_timer_set = time_now;
		alarm(NSECTOSEC(timer_next->time)); 
	}	
}

TIME sectonsec(TIME secs){
    TIME nsecs;
    if ((nsecs = secs * SECTONSEC) && (nsecs/secs != SECTONSEC))
        err_sys("sectonsec error: overflow");
    return nsecs;
}

TIME time(TIME *calptr){ /* time now with nanoseconds granularity */
    struct timespec now;
   
    if (clock_gettime(CLOCK_REALTIME, &now) != 0) {
        err_sys("time error");
    }
    
    TIME nsecs = sectonsec(now.tv_sec);
    
    if ((nsecs += now.tv_nsec ) < 0) {
        err_sys("time error: overflow");
    }

    return (calptr == NULL) ? nsecs : (*calptr = NSECTOSEC(nsecs), nsecs);
}







