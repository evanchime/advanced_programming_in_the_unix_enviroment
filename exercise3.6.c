/*
    *If you open a ﬁle for read–write with the append ﬂag, 
    *can you still read from anywhere in the ﬁle using lseek?
    *Can you use lseek to replace existing data in the ﬁle? 
    *Write a program to verify this.
*/

#include "apue.h" 
#include <fcntl.h> 
#include "standardErrorRoutines.h"

#define PROGRAMOFFSET 3
#define BUFSIZE 10

char buf1[BUFSIZE + 1] ="abcdefghij"; 
char buf2[BUFSIZE + 1] ="ABCDEFGHIJ"; 

int main(void) { 
    
    int fd, n;
    off_t m; 
    char recvline[2];

    if ((fd = open("./exercise3.6.h", O_RDWR | O_APPEND)) < 0) 
        err_sys("open error"); 

    if (write(fd, buf1, BUFSIZE) != BUFSIZE) 
        err_sys("buf1 write error"); 
    /* offset now = BUFSIZE */

    if((n = pread(fd,recvline,PROGRAMOFFSET,SEEK_SET)) == -1){//the lseek and read process. atomic 
        err_sys("pread error");
    }else if(n == 0){
        printf("read is not possible from anywhere\n");
    }
    if(n == PROGRAMOFFSET){
        printf("read is possible from anywhere\n");
    }

    if((n = pwrite(fd,buf2,BUFSIZE,SEEK_SET)) == -1){//the lseek and write process. atomic 
        err_sys("pwrite error");
    }
    if ((m = lseek(fd, 0, SEEK_END)) == -1) 
            err_sys("lseek error");
    else if(m > BUFSIZE){
        printf("you can\'t use lseek to replace existing data in the ﬁle\n offset is %ld bytes\n", m);
    }else{
        printf("you can use lseek to replace existing data in the ﬁle\n offset is %ld bytes\n", m);
    }
    exit(0);
}
    
    


    
    

    
    
    
    


    



