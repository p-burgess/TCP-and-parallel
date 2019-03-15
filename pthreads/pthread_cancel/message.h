#define MSGSZ     256

/*
 * Declare the message structure.
 */

typedef struct msgbuf {
         long int    mtype;
         char    		mtext[MSGSZ];
         } message_buf;

