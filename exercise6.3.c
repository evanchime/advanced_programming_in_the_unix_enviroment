#include "apue.h"
#include "standarderrorroutines.h"
#include <sys/utsname.h>

#define OP_SYSNAME 0x01 /* name of the operating system */
#define OP_NODENAME 0x02 /* name of this node */
#define OP_RELEASE 0x04 /* current release of operating system */
#define OP_VERSION 0x08 /* current version of this release */
#define OP_MACHINE 0x10 /* name of hardware type */

int
main(int argc, char *argv[])
{
	struct utsname name;
	int options;
	int c;

	if(uname(&name) < 0)
		err_sys("uname error");

	if(argc == 1)
		options = OP_SYSNAME | OP_NODENAME | OP_RELEASE | OP_VERSION | OP_MACHINE;
	else{

	}
	exit(0);
}
