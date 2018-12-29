/*
	* Write a program that calls uname and prints all the ﬁelds in the utsname structure. 
	* Compare the output to the output from the uname(1) command.
	* the following is the list of options available from the command line
	* 	-a, --all
          		print all information

        *	-n, --nodename
                	print the network node hostname

        *	-r, --kernel-release
                	print the kernel release

        *	-m, --machine
                	print the machine hardware name

        *	-o, --operating-system
                	print the operating system

        *	-v  --version
                	output version information and exit
*/

#include "apue.h"
#include "standarderrorroutines.h"
#include <sys/utsname.h>

#define OP_SYSNAME 0x01 /* name of the operating system */
#define OP_NODENAME 0x02 /* name of this node */
#define OP_RELEASE 0x04 /* current release of operating system */
#define OP_VERSION 0x08 /* current version of this release */
#define OP_MACHINE 0x10 /* name of hardware type */
#define ALL OP_SYSNAME | OP_NODENAME | OP_RELEASE | OP_VERSION | OP_MACHINE

int
main(int argc, char *argv[])
{
	struct utsname name;
	int options = 0;
	int c;

	if(uname(&name) < 0)
		err_sys("uname error");

	if(argc == 1)
		options = ALL;
	else{
		while (--argc > 0 && (*++argv)[0] == '-'){
     			int flag = 1; // check against illegal option
     			while (c = *++argv[0]){
            			switch (c) {
                			case 'a':
                    				options = ALL;
                    				break;
                			case 'n':
                    				options |= OP_NODENAME;
                    				break;
                			case 'm':
                    				options |= OP_MACHINE;
                    				break;
                			case 'o':
                    				options |= OP_SYSNAME;
                    				break;
                			case 'r':
                    				options |= OP_RELEASE;
                    				break;
                			case 'v':
                    				options |= OP_VERSION;
                    				break;
                			case '-':
                    				if(flag ^= 1)
                					err_sys("uname: illegal option");
						{
                        				char *s = ++argv[0];
                        				if(strcmp(s, "all") == 0)
                            					options = ALL;
                        				else if(strcmp(s, "nodename") == 0)
                            					options |= OP_NODENAME;
                        				else if(strcmp(s, "machine") == 0)
                            					options |= OP_MACHINE;
                        				else if(strcmp(s, "operating-system") == 0)
                            					options |= OP_SYSNAME;
                        				else if(strcmp(s, "version") == 0)
                            					options |= OP_VERSION;
                        				else if(strcmp(s, "release") == 0)
                            					options |= OP_RELEASE;
                        				else
                            					err_sys("uname: illegal option");
                        				argv[0] += (strlen(argv[0]) - 1);
                        				break;
                    				}
                			default:
                    				err_sys("uname: illegal option");
                    				break;
            			}
    			}
		}
	}

	if(options & OP_NODENAME){
    		printf("nodename = %s\n", name.nodename);
	}
	
	if(options & OP_MACHINE){
    		printf("machine = %s\n", name.machine);
	}

	if(options & OP_SYSNAME){
    		printf("operating-system = %s\n", name.sysname);
	}

	if(options & OP_VERSION){
    		printf("version = %s\n", name.version);
	}

	if(options & OP_RELEASE){
    		printf("release = %s\n", name.release);
	}	
	
	exit(0);
}
