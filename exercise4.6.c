/*
	*A program that uses my cp(1) like utility that copies a file containing holes, 
	*without writing the bytes of 0 to the output
*/

#include "standardErrorRoutines.h"
#include "mycp.h"

int
main(int argc, char *argv[])
{
	if (argc != 3) 
		err_quit("usage: a.out <dirname>");
	return mycp(argv[1], argv[2]);
}
