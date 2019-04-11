



void
pr_mask(const char *str) { 
    sigset_t sigset; 
    int errno_save; 
    errno_save = errno; /* we can be called by signal handlers */ 
    if (sigprocmask(0, NULL, &sigset) < 0) { 
        err_ret("sigprocmask error"); 
    }else { 
        printf("%s", str); 
    /*#if defined(SOLARIS)
        char ** siglist = _sys_siglist;
    #else
        char ** siglist = sys_siglist; 
    #endif*/
     int *sigset_char = (int *)&sigset; 
        for(int signo = 0; signo <= 31; signo++)
        {
            if((*sigset_char & (1 << (signo))) == 0)
                continue;
            //printf("%s", /*siglist[signo]*/);
        }
        printf("\n"); 
    } 
    errno = errno_save; /* restore errno */ 
}