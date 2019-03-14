#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>


#include "message.h"

#define KEY			1249

/**
initialize the message queue.
**/
int message_init (void) {
	int msqid;
	int msgflg = IPC_CREAT | 0666;
	key_t key;
	key = KEY;
	msqid = msgget (key,msgflg);
	return msqid;
}

/**
message_send.c -- creating and sending 
to a simple message queue. 

The Message queue is created with a 
basic key and message flag:
 msgflg = IPC_CREAT | 0666 
 -- create queue and make it read and 
 appendable by all. 

A message of type (sbuf.mtype) 1 is sent
to the queue with the message ``Did you 
get this?'

**/

int message_send(char *msg, long int msg_qid)//,int msqid_key)
{
//    int msqid;
//    int msgflg = IPC_CREAT | 0666;
//    key_t key;
    message_buf sbuf;
    size_t buf_length;

    /*
     * Get the message queue id for the
     * "name" 1234, which was created by
     * the server.
     */
//    key = KEY;

//(void) fprintf(stderr, "msgget: Calling msgget(%#lx,%#o)\n",key,msgflg);

/*    if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        return -1;
		  //exit(1);
    }
    else
		 //(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);
*/

    /*
     * We'll send message type 1
     */
     sbuf.mtype = 1;
    
	 //(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);
    
    (void) strcpy(sbuf.mtext, msg);//"Did you get this?");
    
    //(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);
    
    buf_length = strlen(sbuf.mtext) + 1 ;
    
    

    /*
     * Send a message.
     */
    if (msgsnd(msg_qid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
       printf ("%ld, %ld, %s, %d\n", msg_qid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
        return -1;
		  //exit(1);
    }

//   else 
		//printf("Message: \"%s\" Sent\n", sbuf.mtext);
      
    return 0;
	//exit(0);
}

/**
message_rec.c -- receiving the above message

The Message queue is opened with:
msgget (message flag 0666) 
and the same key as message_send.c.

A message of the same type 1 is received 
from the queue with the message 
``Did you get this?'' stored in 
rbuf.mtext. 

Blocks waiting for a message.
**/

int message_recv (char *msg, long int msg_qid)//,int msqid_key)
{
//    int msqid;
//    key_t key;
    message_buf  rbuf;

    /*
     * Get the message queue id for the
     * "name" 1234, which was created by
     * the server.
     */
/*    key = KEY;

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        return -1;
		  //exit(1);
    }
*/
    
    /*
     * Receive an answer of message type 1.
     */
    if (msgrcv(msg_qid/*_key*/, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv");
        return -1;
		  //exit(1);
    }

    /*
     * Print the answer.
     */
//    printf("%s\n", rbuf.mtext);
	 strcpy (msg,rbuf.mtext);
    return strlen (rbuf.mtext);
	 //exit(0);
}
