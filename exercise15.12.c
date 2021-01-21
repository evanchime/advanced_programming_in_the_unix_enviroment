/*
	* Write  a  program  that  does  the  following.   Execute  a  loop  five  times:  create  a  messagequeue,  
	* print  the  queue  identifier,delete  the  message  queue.   
	* Then  execute  the  next  loopfive  times:  create  a  message  queue  with  a  key  of IPC_PRIVATE, 
	* and  place  a  message  onthe  queue.   After  the  program  terminates,  look  at  the  message  queues  usingipcs(1).
	* Explain what is happening with the queue identifiers 
*/ 

#include "apue.h" 
#include "standarderrorroutines.h"
#include <sys/msg.h>

#define KEY 0x125 // key for first message queue
#define MAXMSZ  512     /* maximum message size */

struct mymesg{
	long mtype;
	char mtext [MAXMSZ];
};

int 
main(void)
{
	int qid = 0;
	size_t nbytes;
        struct mymesg m;
	
	for(int i = 1; i <= 5; i++){
		if ((qid = msgget((KEY + i), IPC_CREAT | 0666)) < 0)
			err_sys("msgget error");
		printf("queue ID %d is %d\n", i, qid);
		if(msgctl(qid, IPC_RMID, NULL) < 0)
			err_sys("msgctl error");
	}

	for(int i = 1; i <= 5; i++){
		if ((qid = msgget(IPC_PRIVATE, 0666)) < 0)
			err_sys("msgget error");
		memset(&m, 0, sizeof(m));
		sprintf(m.mtext, "%s%d", "Hello world ", i);
		nbytes = strlen(m.mtext);
		m.mtype = 1;
		if (msgsnd(qid, &m, nbytes, 0) < 0)
			err_sys("can’t send message");
	}

	exit(0); // parent terminates
}
