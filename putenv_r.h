/*
	* Implement putenv_r,areentrant version of putenv.
	* Make surethat your implementation is async-signal safe as well as thread-safe.
*/

#include "apue.h"
#include "standarderrorroutines.h" 
#include <pthread.h> 

extern char **environ; 

pthread_mutex_t env_mutex; 

static pthread_once_t init_done = PTHREAD_ONCE_INIT; 

static void 
thread_init(void) 
{ 
	pthread_mutexattr_t attr; 
	pthread_mutexattr_init(&attr); 
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); 
	pthread_mutex_init(&env_mutex, &attr); 
	pthread_mutexattr_destroy(&attr); 
}

int 
putenv_r(char *str)
{
	sigset_t newmask, oldmask; 
	
	/* 
		*Block signal and save current signal mask.
	*/ 
	sigfillset(&newmask); 
	if (pthread_sigmask(SIG_BLOCK, &newmask, &oldmask) < 0) 
		err_sys("SIG_BLOCK error"); 
	
	pthread_once(&init_done, thread_init);
	pthread_mutex_lock(&env_mutex);
	
	int n;

	if((n = putenv(str)) != 0){
		pthread_mutex_unlock(&env_mutex); 
		return n;
	}
	pthread_mutex_unlock(&env_mutex);

	/* 
		*Restore signal mask which unblocks all signals. 
	*/ 
	if (pthread_sigmask(SIG_SETMASK, &oldmask, NULL) < 0) 
		err_sys("SIG_SETMASK error");

	return n;
}

