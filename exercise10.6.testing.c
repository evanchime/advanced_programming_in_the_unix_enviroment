#include "apue.h" 
#include <fcntl.h>
#include <string.h>
#include "standarderrorroutines.h"


static volatile sig_atomic_t sigflag; /* set nonzero by sig handler */ 
static sigset_t newmask, oldmask, zeromask;

void write_to_file(int fd, char buf[], int counter, const char *str);
void itoa(int n, char s[]);
void reverse(char s[]);
void WAIT_CHILD(void);
void TELL_WAIT(void);
void TELL_CHILD(pid_t pid);
void WAIT_PARENT(void);
void TELL_PARENT(pid_t pid); 
static void sig_usr(int signo);
void pr_mask(const char *str);
//Sigfunc * signal(int signo, Sigfunc *func);

int 
main(void) 
{ 

    TELL_WAIT();
    int fd;
    int counter = 0; 
    pid_t pid;
    char buf[MAXLINE]; 

    if ((fd = open("hello", O_WRONLY | O_CREAT, FILE_MODE)) < 0) {
        err_sys("open error");
    }

    write_to_file(fd, buf, counter++, "parent");

    if ((pid = fork()) < 0) { 
        err_sys("fork error"); 
    }else if (pid == 0) { // child
        while(1){
            write_to_file(fd, buf, counter++, "child");
            TELL_PARENT(getppid());
            WAIT_PARENT();
        }
    }else {  // parent 
        while(1){
            WAIT_CHILD();
            write_to_file(fd, buf, counter++, "parent");
            TELL_CHILD(pid);
        }
    } 
}

static 
void sig_usr(int signo) /* one signal handler for SIGUSR1 and SIGUSR2 */ 
{ 
    sigflag = 1; 
} 

void 
TELL_WAIT(void) { 
    if (signal(SIGUSR1, sig_usr) == SIG_ERR) 
        err_sys("signal(SIGUSR1) error"); 
    if (signal(SIGUSR2, sig_usr) == SIG_ERR) 
        err_sys("signal(SIGUSR2) error"); 
    sigemptyset(&zeromask); 
    sigemptyset(&newmask); 
    sigaddset(&newmask, SIGUSR1); 
    sigaddset(&newmask, SIGUSR2); 
    /* Block SIGUSR1 and SIGUSR2, and save current signal mask */ 
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) 
        err_sys("SIG_BLOCK error"); 
} 

void 
TELL_PARENT(pid_t pid) { 
    kill(pid, SIGUSR2); /* tell parent we’re done */ 
} 

void 
WAIT_PARENT(void) { 
    while (sigflag == 0) 
        sigsuspend(&zeromask); /* and wait for parent */ 
    sigflag = 0; 
    /* Reset signal mask to original value */ 
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) 
        err_sys("SIG_SETMASK error"); 
} 

void 
TELL_CHILD(pid_t pid) { 
    kill(pid, SIGUSR1); /* tell child we’re done */ 
} 

void 
WAIT_CHILD(void){
    while (sigflag == 0) 
        sigsuspend(&zeromask); /* and wait for child */ 
    sigflag = 0; 
    /* Reset signal mask to original value */ 
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) 
        err_sys("SIG_SETMASK error");
}

/* reverse:  reverse string s in place */
void 
reverse(char s[])
{
    int c, i, j;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* itoa:  convert n to characters in s */
void 
itoa(int n, char s[])
{
    int i, sign;
    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void 
write_to_file(int fd, char buf[], int counter, const char *str)
{
    itoa(counter, buf);
    size_t n = strlen(buf);
    buf[n] = '\n';
    buf[++n] = '\0';
    if (write(fd, buf, n) != n) 
        err_sys("counter write error");
    printf("%s, writes counter value: %d to file\n ", str, counter);
}


void 
pr_mask(const char *str) { 
    sigset_t sigset; 
    int errno_save; 
    errno_save = errno; /* we can be called by signal handlers */ 
    if (sigprocmask(0, NULL, &sigset) < 0) { 
        err_ret("sigprocmask error"); 
    }else { 
        printf("%s", str); 
        if (sigismember(&sigset, SIGINT)) 
            printf(" SIGINT"); 
        if (sigismember(&sigset, SIGQUIT)) 
            printf(" SIGQUIT"); 
        if (sigismember(&sigset, SIGUSR1)) 
            printf(" SIGUSR1"); 
        if (sigismember(&sigset, SIGUSR2)) 
            printf(" SIGUSR2");
        if (sigismember(&sigset, SIGALRM)) 
            printf(" SIGALRM"); 
        /* remaining signals can go here */ 
        printf("\n"); 
    } 
    
    errno = errno_save; /* restore errno */ 
}

/* Reliable version of signal(), using POSIX sigaction(). */ 
/*Sigfunc 
* signal(int signo, Sigfunc *func) { 
    struct sigaction act, oact; 
    act.sa_handler = func; 
    sigemptyset(&act.sa_mask); 
    act.sa_flags = 0; 
    if (signo == SIGALRM) { 
#ifdef SA_INTERRUPT 
        act.sa_flags |= SA_INTERRUPT; 
#endif 
    }else { 
        act.sa_flags |= SA_RESTART; 
    }
    act.sa_flags |= SA_RESTART; 
    if (sigaction(signo, &act, &oact) < 0) 
        return(SIG_ERR); 
    return(oact.sa_handler); 
}*/