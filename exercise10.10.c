/*
	* Write a program that calls sleep(60) in an inﬁnite loop. 
	* Every ﬁve times through the loop (every 5 minutes), fetch the current time of day and print the tm_sec ﬁeld. 
	* Run the program overnight and explain the results. 
	* How would a program such as the cron daemon, which runs every minute on the minute, handle this situation?
*/

#include "apue.h" 
#include <time.h> 
#include <sys/time.h>
#include "standarderrorroutines.h"

#ifndef CLOCK_REALTIME 
#define CLOCK_REALTIME 0 
#define USECTONSEC 1000 /* microseconds to nanoseconds */ 
void 
clock_gettime(int id, struct timespec *tsp) { 
	struct timeval tv; 
	gettimeofday(&tv, NULL); 
	tsp->tv_sec = tv.tv_sec; 
	tsp->tv_nsec = tv.tv_usec * USECTONSEC; 
} 
#endif

void
pr_tm_sec(){
	struct timespec tsp;
	struct tm *tmp;

	clock_gettime(CLOCK_REALTIME, &tsp);
        tmp = localtime(&tsp.tv_sec);
	printf("%d\n", tmp->tm_sec);
}

int
main()
{
	int times = 0;
	while(1){
		if(times++ != 5){
			sleep(60);
		}else{
			times = 0;
			pr_tm_sec();
		}
	}
	exit(0);
}
