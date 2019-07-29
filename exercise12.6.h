/*
	* Reimplement the program in Figure10.29 to make it thread-safe without using nanosleeporclock_nanosleep.
*/

/* 
	* Figure10.29 shows an implementation of the POSIX.1 sleep function. 
	* This function is amodiﬁcation of Figure10.7, which handles signals reliably,avoiding the race condition in the earlier implementation. 
	* We still do not handle any interactions with previously set alarms. (As we mentioned, these interactions are explicitly undeﬁned by POSIX.1.)
*/

/* 
	* I tested it in exercise12.6.c with 10 threads, but not more, and it works
	* letting me know of any bugs discovered will be highly appreciated
*/ 

#include "apue.h"
#include "standarderrorroutines.h"
#include <pthread.h>
#include <errno.h>

static void 
sig_alrm(int signo) 
{ 
	/* nothing to do */ 
}


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

unsigned int
sleep(unsigned int seconds) 
{
	//int signop, err;
	/*if ((err = pthread_mutex_lock(&lock)) != 0)
                err_exit(err, "pthread_mutex_unlock error");*/

	struct sigaction newact, oldact; 
	sigset_t newmask, oldmask; 
	unsigned int unslept; 
	int signop, err;
	
	/* set our handler, save previous information */ 
	newact.sa_handler = sig_alrm; 
	sigemptyset(&newact.sa_mask); 
	newact.sa_flags = 0; 
	sigaction(SIGALRM, &newact, &oldact); 
	
	/* block SIGALRM and save current signal mask */ 
	sigemptyset(&newmask); 
	sigaddset(&newmask, SIGALRM); 
	if ((err = pthread_sigmask(SIG_BLOCK, &newmask, &oldmask)) != 0) 
		err_exit(err, "SIG_BLOCK error");
	
	if ((err = pthread_mutex_lock(&lock)) != 0)
                err_exit(err, "pthread_mutex_unlock error");
	alarm(seconds); 
	
	/* wait for SIGALRM to occur */ 
	sigwait(&newmask, &signop);

	/* some signal has been caught, SIGALRM is now blocked */ 
	unslept = alarm(0); 

	if ((err = pthread_mutex_unlock(&lock)) != 0)
                err_exit(err, "pthread_mutex_unlock error");

	/* reset previous action */ 
	sigaction(SIGALRM, &oldact, NULL); 
	
	/* reset signal mask, which unblocks SIGALRM */ 
	if ((err = pthread_sigmask(SIG_SETMASK, &oldmask, NULL)) != 0) 
		err_exit(err, "SIG_SETMASK error");

	/*if ((err = pthread_mutex_unlock(&lock)) != 0)
                err_exit(err, "pthread_mutex_unlock error");*/

	return(unslept); 
}

