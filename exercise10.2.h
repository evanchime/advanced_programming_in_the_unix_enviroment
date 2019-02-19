#ifndef _SIG2STR_H 
#define _SIG2STR_H

/*
    *Implement the sig2str function described in Section 10.22.
    *this implementation is for solaris
*/

#include "apue.h"
#include "standarderrorroutines.h"

#define NSIG 36 + 1

#define SIGBAD(signo) ((signo) <= 0 || (signo) >= NSIG)


/*
    * The caller must ensure that the memory is large enough to hold the longest string, including the terminating null byte. 
    * Solaris provides the constant SIG2STR_MAX in <signal.h> to deﬁne the maximum string length.
*/

int sig2str(int signo, char *str){

    if (SIGBAD(signo)) {
        return(-1); 
    }

    switch (signo)
    {
        case 1:
            memmove(str, "HUP", sizeof("HUP"));
            break;

        case 2:
            memmove(str, "INT", sizeof("INT"));
            break;

        case 3:
            memmove(str, "QUIT", sizeof("QUIT"));
            break;

        case 4:
            memmove(str, "ILL", sizeof("ILL"));
            break;

        case 5:
            memmove(str, "TRAP", sizeof("TRAP"));
            break;

        case 6:
            memmove(str, "ABRT", sizeof("ABRT"));
            break;

        case 7:
            memmove(str, "EMT", sizeof("EMT"));
            break;

        case 8:
            memmove(str, "FPE", sizeof("FPE"));
            break;

        case 9:
            memmove(str, "KILL", sizeof("KILL"));
            break;

        case 10:
            memmove(str, "BUS", sizeof("BUS"));
            break;

        case 11:
            memmove(str, "SEGV", sizeof("SEGV"));
            break;

        case 12:
            memmove(str, "SYS", sizeof("SYS"));
            break;

        case 13:
            memmove(str, "PIPE", sizeof("PIPE"));
            break;

        case 14:
            memmove(str, "ALRM", sizeof("ALRM"));
            break;

        case 15:
            memmove(str, "TERM", sizeof("TERM"));
            break;

        case 16:
            memmove(str, "USR1", sizeof("USR1"));
            break;

        case 17:
            memmove(str, "USR2", sizeof("USR2"));
            break;

        case 18:
            memmove(str, "CHLD", sizeof("CHLD"));
            break;

        case 19:
            memmove(str, "PWR", sizeof("PWR"));
            break;

        case 20:
            memmove(str, "WINCH", sizeof("WINCH"));
            break;

        case 21:
            memmove(str, "URG", sizeof("URG"));
            break;

        case 22:
            memmove(str, "POLL", sizeof("POLL"));
            break;

        case 23:
            memmove(str, "STOP", sizeof("STOP"));
            break;

        case 24:
            memmove(str, "TSTP", sizeof("TSTP"));
            break;

        case 25:
            memmove(str, "CONT", sizeof("CONT"));
            break;

        case 26:
            memmove(str, "TTIN", sizeof("TTIN"));
            break;

        case 27:
            memmove(str, "TTOU", sizeof("TTOU"));
            break;

        case 28:
            memmove(str, "VTALRM", sizeof("VTALRM"));
            break;

        case 29:
            memmove(str, "PROF", sizeof("PROF"));
            break;

        case 30:
            memmove(str, "XCPU", sizeof("XCPU"));
            break;

        case 31:
            memmove(str, "XFSZ", sizeof("XFSZ"));
            break;

        case 32:
            memmove(str, "WAITING", sizeof("WAITING"));
            break;

        case 33:
            memmove(str, "LWP", sizeof("LWP"));
            break;

        case 34:
            memmove(str, "FREEZE", sizeof("FREEZE"));
            break;

        case 35:
            memmove(str, "THAW", sizeof("THAW"));
            break;

        case 36:
            memmove(str, "CANCEL", sizeof("CANCEL"));
            break;

        default:
            break;
    }
    
    return 0;
}

#endif /* _SIG2STR_H */










        
    

    




