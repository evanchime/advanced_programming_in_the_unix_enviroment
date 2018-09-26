#include "standardErrorRoutines.h"
#include "path_alloc.h"

int
main(void)
{
	size_t size, size_copy ;  // to return value of system PATH_MAX and make a copy of it

	char *buf; // buffer for getcwd call


        buf = path_alloc(&size);

	size_copy = size;

	while(size-- != 0){
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
