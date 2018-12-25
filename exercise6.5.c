/*
	* Write a program to obtain the current time and print it using strftime
	* so that it looks like the default output from date(1).
	* Set the TZ environment variable to different values and see what happens.
*/

#include "apue.h"
#include "standarderrorroutines.h"
#include <time.h>
#include <sys/time.h>

int
main(void)
{
	struct timespec tsp; 
	struct tm *tmp; 
	char buf[MAXLINE];

	if(clock_gettime(CLOCK_REALTIME, &tsp) < 0)
		err_sys("clock_gettime error");
	
	if((tmp = localtime(&tsp.tv_sec)) == NULL)
		err_sys("localtime error");

	if (strftime(buf, MAXLINE, "%a %e %b %T %Z %Y", tmp) == 0)
	       err_sys("strftime error");	
		
	printf("%s\n", buf);
	
	exit(0);
}
