/*
	* Rewrite the function in Figure10.14 to handle all the signals from Figure10.1. 
	* The function should consist of a single loop that iterates once for every signal in the current signal mask (not once for every possible signal).
	* These function is not portable. Works on my linux x86-64 machine
	* on these machine SIGABRT and SIGIOT have the same signo. 
*/

char *sys_siglist[] = {
			"SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1",
				"SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP",
		       "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU", "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGIO", "SIGPWR", "SIGSYS"
};


void
pr_mask(const char *str) { 
    sigset_t sigset; 
    int errno_save; 
    errno_save = errno; /* we can be called by signal handlers */ 
    if (sigprocmask(0, NULL, &sigset) < 0) { 
        err_ret("sigprocmask error"); 
    }else { 
        printf("%s", str); 
    	for(int signo = 1; signo < 32; signo++)
    	{
    		if(sigismember(&sigset, signo))
                	printf(" %s", sys_siglist[signo - 1]);
        }
        printf("\n");	
    }
    errno = errno_save; /* restore errno */ 
}
