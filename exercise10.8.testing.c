#include "apue.h" 
#include "standarderrorroutines.h"
#include "Untitled-1.h"

sigset_t newmask, oldmask;
//void pr_mask(const char *);

struct my_sigset_t
{
    int sigset;
};

int main(int argc, char const *argv[])
{
    /*sigemptyset(&newmask); 
    sigaddset(&newmask, SIGUSR1); 
    sigaddset(&newmask, SIGUSR2); */
    /* Block SIGUSR1 and SIGUSR2, and save current signal mask */ 
    /*if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) 
        err_sys("SIG_BLOCK error"); */
    //pr_mask("main starts");
    /* Reset signal mask to original value */ 
    /*if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) 
        err_sys("SIG_SETMASK error"); 

    printf("%d", newmask);*/

    struct my_sigset_t sig_set = {0};
    int x = sig_set | 1;
    //*x |= 1;
    printf("%d", x);
    
    return 0;
}




