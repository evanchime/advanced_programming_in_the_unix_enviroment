#include "standardErrorRoutines.h"
#include "mycp.h"

int
main(int argc, char *argv[])
{
	if (argc != 3) 
		err_quit("usage: a.out <dirname>");
	return mycp(argv[1], argv[2]);
}
