#include "apue.h" 
#include <fcntl.h>
#include <string.h>
#include "standarderrorroutines.h"

/* reverse:  reverse string s in place */
void reverse(char s[])
{
    int c, i, j;
    for (i = 0, j = strlen(s)−1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* itoa:  convert n to characters in s */
void itoa(int n, char s[])
{
    int i, sign;
    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + ′0′;   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = ′-′;
    s[i] = ′\0′;
    reverse(s);
}

void write_to_file(int fd, char buf[], int *counter)
{
    itoa(*counter++, buf);
    size_t n = strlen(buf);
    if (write(fd, buf, n) != n) 
        err_sys("counter write error");
    printf("%d, increments counter to %d\n", getpid(), *counter);
}

int 
main(void) 
{ 
    int fd;
    int counter = 50; 
    pid_t pid; 

    if ((fd = open("hello", O_WRONLY | O_CREAT, FILE_MODE)) < 0) {
        err_sys("open error");
    }

    write_to_file(fd, &counter);
    
    
    
    
    
    exit(0); 
}
