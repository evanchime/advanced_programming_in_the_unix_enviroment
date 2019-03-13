#include "apue.h"
#include "standarderrorroutines.h"
#include <time.h>

#define TRUE	1
#define FALSE	0

#define MAX_TIMERS	10	/* number of timers */
typedef time_t TIME;		/* how time is actually stored */
#define VERY_LONG_TIME	(1  << ((sizeof(TIME) << 3) - 1)) - 1	/* longest time possible */

struct timer {
	int inuse;		/* TRUE if in use */
	TIME time;		/* relative time to wait */
	char *event;		/* set to TRUE at timeout */
} timers[MAX_TIMERS];		/* set of timers */

struct timer *timer_next = NULL;/* timer we expect to run down next */
TIME time_timer_set;		/* time when physical timer was set */
sigset_t newmask, oldmask;      /* for signal masks */

void timers_update();		/* subtract time from all timers, enabling any that run out along the way */
static void sig_alrm(int);     /* SIGALRM handler */
void disable_sigalrm_interrupts(void); /* disable SIGALRM */
void enable_sigalrm_interrupts(void); /* enable SIGALRM */

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
		timers_update(time(NULL) - time_timer_set);
		if (timer_next) {
			alarm(timer_next->time);
			time_timer_set = time(NULL);
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
		timers_update(alarm(time_));
		enable_sigalrm_interrupts();
		return 0;
	}
	
	for (t=timers;t<&timers[MAX_TIMERS];t++) {
		if (!t->inuse) break;
	}

	/* out of timers? */
	if (t == &timers[MAX_TIMERS]) {
		enable_sigalrm_interrupts();
		return(0);
	}

	/* install new timer */
	t->event = event;
	t->time = time_;
	if (!timer_next) {
		/* no timers set at all, so this is shortest */
		time_timer_set = time(NULL);
		alarm((timer_next = t)->time);
	} else if ((time_ + time(NULL)) < (timer_next->time + time_timer_set)) {
		/* new timer is shorter than current one, so */
		timers_update(time(NULL) - time_timer_set);
		time_timer_set = time(NULL);
		alarm((timer_next = t)->time);
	} else {
		/* new timer is longer, than current one */
	}
	t->inuse = TRUE;
	enable_sigalrm_interrupts();
	return(t);
}

static void 
sig_alrm(int signo) { 
	timers_update(time(NULL) - time_timer_set);
	/* start physical timer for next shortest time if one exists */
	if (timer_next) {
		time_timer_set = time(NULL);
		alarm(timer_next->time);
	}	
}

int 
main(int argc, char const *argv[])
{

	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		err_sys("can’t catch SIGALRM");

	timers_init(); 

	timer_declare(7, "first timer");
	timer_declare(4, "second timer");
	timer_declare(12, "third timer");

	//printf("%ld", ((long)1  << ((sizeof(long) << 3) - 1)) - 1);
	//printf("%ld", sizeof(long));
	//printf("%ld", ((TIME)1 << (sizeof(TIME) << 3) - 1));
	return 0;
}
