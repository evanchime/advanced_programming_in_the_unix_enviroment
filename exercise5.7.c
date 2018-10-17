/*
 	* BSD-based systems provide a function called funopen 
	* that allows us to intercept read, write, seek, and close calls on a stream. 
	* Use this function to implement fmemopen for FreeBSD and Mac OS X
*/

#include "standarderrorroutines.h"
#include "myfmemopen_io.h"
#include "myfmemopen.h"

#define BSZ 48 

int 
main() 
{ 
	FILE *fp; 
	char buf[BSZ]; 
	
	memset(buf, 'a', BSZ-2); 
	buf[BSZ-2] = '\0'; 
	buf[BSZ-1] = 'X'; 
	if ((fp = fmemopen(buf, BSZ, "w+")) == NULL) 
		err_sys("fmemopen failed"); 
	printf("initial buffer contents: %s\n", buf); 
	fprintf(fp, "hello, world"); 
	printf("before flush: %s\n", buf); 
	fflush(fp); 
	printf("after fflush: %s\n", buf); 
	printf("len of string in buf = %ld\n", (long)strlen(buf)); 
	
	memset(buf, 'b', BSZ-2); 
	buf[BSZ-2] = '\0'; 
	buf[BSZ-1] = 'X'; 
	fprintf(fp, "hello, world"); 
	fseek(fp, 0, SEEK_SET); 
	printf("after fseek: %s\n", buf); 
	printf("len of string in buf = %ld\n", (long)strlen(buf));

	memset(buf, 'c', BSZ-2); 
	buf[BSZ-2] = '\0'; 
	buf[BSZ-1] = 'X'; 
	fprintf(fp, "hello, world"); 
	fclose(fp); 
	printf("after fclose: %s\n", buf); 
	printf("len of string in buf = %ld\n", (long)strlen(buf)); 
	
	return(0);
}

