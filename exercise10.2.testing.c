#include "apue.h"
#include "standarderrorroutines.h"

int sig2str(int signo, char *str);

int 
main(int argc, char const *argv[])
{
    /* code */
    
    char *x = malloc(10);
    x = memmove(x, "w", sizeof("w"));

    printf("%s", x);

    free(x);
    exit(0);
}

int sig2str(int signo, char *str){
    
    if ((strlen(str) + 1) != 32/*SIG2STR_MAX*/) // the constant SIG2STR_MAX deﬁnes the maximum string length.
        return -1;

    switch (signo)
    {
        case /* constant-expression */:
            /* code */
            break;

        case /* constant-expression */:
            /* code */
            break;

        case /* constant-expression */:
            /* code */
            break;

        case /* constant-expression */:
            /* code */
            break;

        case /* constant-expression */:
            /* code */
            break;

        case /* constant-expression */:
            /* code */
            break;

        case /* constant-expression */:
            /* code */
            break;

        case /* constant-expression */:
            /* code */
            break;

        case /* constant-expression */:
            /* code */
            break;
    
        default:
            break;
    }
    
}


