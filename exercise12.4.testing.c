#include <limits.h> 
#include "apue.h"
#include "standarderrorroutines.h"
#include <pthread.h> 

#define MAXSTRINGSZ 4096 

static pthread_key_t key; 
static pthread_once_t init_done = PTHREAD_ONCE_INIT; 
pthread_mutex_t env_mutex = PTHREAD_MUTEX_INITIALIZER;

extern char **environ; 

static void 
thread_init(void) 
{ 
	pthread_key_create(&key, free); 
} 

char * 
getenv(const char *name) 
{ 
	int i, len; 
	char *envbuf; 
	
	pthread_once(&init_done, thread_init); 
	pthread_mutex_lock(&env_mutex); 
	envbuf = (char *)pthread_getspecific(key); 
	if (envbuf == NULL) { 
		envbuf = malloc(MAXSTRINGSZ); 
		if (envbuf == NULL) { 
			pthread_mutex_unlock(&env_mutex); 
			return(NULL); 
		} 
		pthread_setspecific(key, envbuf); 
	} 
	len = strlen(name); 
	for (i = 0; environ[i] != NULL; i++) { 
		if ((strncmp(name, environ[i], len) == 0) && (environ[i][len] == '=')) { 
			strncpy(envbuf, &environ[i][len+1], MAXSTRINGSZ-1); 
			pthread_mutex_unlock(&env_mutex); 
			return(envbuf); 
		} 
	} 
	pthread_mutex_unlock(&env_mutex); 
	return(NULL); 
}

void * 
thr_fn(void *arg) 
{
	printf("start of thread\n");
	char *envbuf;
        if((envbuf = getenv(arg)) != NULL)
                printf("%s from thread\n", envbuf);
        else
                printf("Not found\n");
	printf("end of thread\n");
	return ((void *)0);
}

int 
main(int argc, char *argv[])
{
	printf("start of main thread\n");
	if (argc != 2) 
		err_quit("usage: a.out <enviroment variable>");
	int err;
	pthread_t tid;

	err = pthread_create(&tid, NULL, thr_fn, argv[1]);
	if (err != 0) 
		err_exit(err, "can’t create thread");

	sleep(1);
	char *envbuf;
       	if((envbuf = getenv(argv[1])) != NULL)
		printf("%s from main thread\n", envbuf);
	else
		printf("Not found\n");
	printf("end of main thread\n");
	exit(0);
}
