#include <stdio.h>



int
main(int argc, char *argv[])
{
	FILE *fp;
	if ((fp = tmpfile()) == NULL) /* create temp file */ 
		err_sys("tmpfile error");
}

