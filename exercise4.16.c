/*
 	*Does the UNIX System have a fundamental limitation on the depth of a directory tree? 
	*To ﬁnd out, write a program that creates a directory and then changes to that directory, in a loop. 
	*Make certain that the length of the absolute pathname of the leaf of this directory is greater than your system’s PATH_MAX limit. 
	*Can you call getcwd to fetch the directory’s pathname?
*/ 

#include "standardErrorRoutines.h"
#include "path_alloc.h"

int
main(void)
{
	size_t size, size_copy ;  // to return value of system PATH_MAX and make a copy of it

	char *buf; // buffer for getcwd call


        buf = path_alloc(&size); // return value of PATH_MAX, and allocate buffer of PATH_MAX size

	size_copy = size;

	while(size-- != 0){ // creates a directory and then changes to that directory, in a loop
		if(mkdir("a", DIR_MODE) < 0){
			err_sys("mkdir failed");
		}	
		if(chdir("a") < 0){
			err_sys("chdir failed");
		}
	}

	while(getcwd(buf, size_copy) == NULL){// call getwcd until it succeeds
		if(errno == ERANGE){
			size_copy *= 2;
			if ((buf = realloc(buf,size_copy)) == NULL) 
				err_sys("realloc failed");
		}
		else
			err_sys("getcwd failed");
	}

	exit(0);
}
