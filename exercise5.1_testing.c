#include <stdio.h>
#include "standarderrorroutines.h"
#include "apue.h"
#include "exercise5.1.h"

int
main(int argc, char *argv[])
{
	FILE *fp;
	if ((fp = tmpfile()) == NULL) /* create temp file */ 
		err_sys("tmpfile error");

	char buf[BUFSIZ];

	fp = stdout;


	my_setbuf(/*stdout*/fp, buf);

	printf("%d",/*stdout*/fp->_bf._size);
	exit(0);
}

