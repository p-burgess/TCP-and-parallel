#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "message.c"
#include "control.c"
#include "queue.c"
#include "dbug.h"
#include "setup_socket.c"

#define log_file "./log.text"
//#include "networking.c"
//#include "log.c"

int end_server;
int end_logger;

/* the work_queue holds tasks for the various threads to complete. */
struct work_queue {
	data_control control;
	queue work;
} wq;

/* I added a job number to the work node.  Normally, the work node
   would contain additional data that needed to be processed. */
typedef struct work_node {
	struct node *next;
	int jobnum;
	char *req;
	FILE *resp;
	int client_fd;
	struct tm * time;
	int end_server;
} wnode;

/* the cleanup queue holds stopped threads.  Before a thread
   terminates, it adds itself to this list.  Since the main thread is
   waiting for changes in this list, it will then wake up and clean up
   the newly terminated thread. */
struct cleanup_queue {
	data_control control;
	queue cleanup;
} cq;

/* I added a thread number (for debugging/instructional purposes) and
   a thread id to the cleanup node.  The cleanup node gets passed to
   the new thread on startup, and just before the thread stops, it
   attaches the cleanup node to the cleanup queue.  The main thread
   monitors the cleanup queue and is the one that performs the
   necessary cleanup. */
typedef struct cleanup_node {
	struct cnode *next;
	int threadnum;
	int msg_qid;
	char *t_desc;	
	pthread_t tid;
	long int t_id;//pthread_id : (syscall(224))
//	int end_server;
} cnode;

void *logger(void *myarg) {
//	wnode *mywork;
	cnode *mynode;
	mynode=(cnode *) myarg;
	mynode->t_id=syscall(224);
	mynode->t_desc="logger";
//	mynode->end_server=0;

	time_t currentTime;
	struct tm * Time;
//	char log_entry[MSGSZ+32];
	printf ("thread: %d %s start TID: %ld\n",mynode->threadnum,mynode->t_desc,(long int)mynode->t_id);

	currentTime=time (NULL);
	Time = localtime ( &currentTime );

/*	FILE *log_fd=0;
	if ((log_fd = fopen ("./log.text","a+")) == NULL)
		fprintf(stderr,"Unable to open log file for writing: %d\n",log_fd);

	sprintf (log_entry,":%d:",Time->tm_year+1900);
	sprintf (log_entry,"%d:",Time->tm_mon+1);
	sprintf (log_entry,"%d:",Time->tm_mday+1);
	sprintf (log_entry,"%d:",Time->tm_hour);
	sprintf (log_entry,"%d:",Time->tm_min);
	sprintf (log_entry,"%d:",Time->tm_sec);
	sprintf (log_entry,"logger started\n");
	fprintf (log_fd,log_entry);*/

	char *msg=(char *) malloc (sizeof (MSGSZ));
	while (1) {
		message_recv (msg,mynode->msg_qid);//blocks for message
		currentTime=time (NULL);
		Time = localtime ( &currentTime );
		printf ("%s : %s\n",asctime(Time),msg);
		//fprintf (log_fd,"%s",msg);
		//^why doesn't this work?
	}
	
//	fclose (log_fd);
	sleep (1);
	pthread_mutex_lock(&cq.control.mutex);
	queue_put(&cq.cleanup,(node *) mynode);
	pthread_mutex_unlock(&cq.control.mutex);
	pthread_cond_signal(&cq.control.cond);
	printf("thread %ld shutting down...\n",mynode->t_id);
	printf ("thread: %d %s stop TID: %ld",mynode->threadnum,mynode->t_desc,(long int)mynode->t_id);
	sleep (1);
	return NULL;
}

int queue_connection (int x,int newclient_fd,char *buffer)
{
	wnode *mywork;
	mywork=malloc(sizeof(wnode));
	
	if (!mywork) {
		printf("ouch! can't malloc!\n");
		return -1;
	}

	mywork->jobnum=x;
	mywork->client_fd = newclient_fd;
	mywork->req = buffer;

	pthread_mutex_lock(&wq.control.mutex);

	queue_put(&wq.work,(node *) mywork);
	
	pthread_mutex_unlock(&wq.control.mutex);
	pthread_cond_broadcast(&wq.control.cond);
	return 1;
}

void *responder (void *myarg) 
{
	cnode *mynode;
	mynode=(cnode *) myarg;
	mynode->t_id=syscall(224);
	mynode->t_desc="responder";

	wnode *mywork;

	char *msg=(char *) malloc (sizeof (MSGSZ));
	sprintf (msg,"thread: %d %s start TID: %ld",mynode->threadnum,mynode->t_desc,(long int)mynode->t_id);
	message_send (msg,mynode->msg_qid);
	
	
	while (end_server==0&&wq.control.active) {
		
		while (wq.work.head==NULL&&wq.control.active) {
			pthread_cond_wait (&wq.control.cond,&wq.control.mutex);
		}
		if (!wq.control.active)
			break;
		//we got something one queue_get
		mywork=(wnode *) queue_get (&wq.work);
		pthread_mutex_unlock (&wq.control.mutex);
		//perform processing
		// send mywork->resp(FILE *) to mywork->client_fd(int)
		printf ("%s\n",mywork->req);
		free (mywork);
		pthread_mutex_lock (&wq.control.mutex);
		
	}
//	sleep (1);

sprintf (msg,"thread: %d %s stop TID: %ld",mynode->threadnum,mynode->t_desc,(long int)mynode->t_id);
	message_send (msg,mynode->msg_qid);
	
	pthread_mutex_lock(&cq.control.mutex);
	queue_put(&cq.cleanup,(node *) mynode);
	pthread_mutex_unlock(&cq.control.mutex);
	pthread_cond_signal(&cq.control.cond);
	printf("thread %d shutting down...\n",mynode->threadnum);
	return NULL;
}
void *accepter (void *myargs) 
{
	//	wnode *mywork;
	cnode *mynode;
	mynode=(cnode *) myargs;
	mynode->t_id=syscall(224);
	mynode->t_desc="accepter";
	
	int i,rc,j,x=0;
	int max_sd;
	int desc_ready, end_server = 0;
	int close_conn;
	void *buffer;
	
	struct timeval timeout;
	fd_set master_set,read_fd;
	
	char *msg=(char *) malloc (sizeof (MSGSZ));
	sprintf (msg,"thread: %d %s start TID: %ld",mynode->threadnum,mynode->t_desc,(long int)mynode->t_id);
	message_send (msg,mynode->msg_qid);
	
	FD_ZERO (&master_set);
	max_sd = setup_sock ();
	FD_SET (max_sd,&master_set);
	//	fprintf (stderr,"FD_ZERO set\n");
	
	timeout.tv_sec = 30;
	timeout.tv_usec = 0;
	/* Loop for incoming connections */
	do {
		memcpy (&read_fd,&master_set, sizeof (master_set));
		rc = select (max_sd+1,&read_fd,NULL,NULL,&timeout);
		if (rc < 0)
			break;
		if (rc == 0)
			break;
		desc_ready = rc;
		for (i=0;i<=max_sd && desc_ready > 0;++i) {
			if (FD_ISSET (i,&read_fd)) {
				desc_ready -= 1;
				if (i == max_sd) {
					do {
						fprintf (stderr,"connection accepted\n");
						j = accept (max_sd,NULL,NULL);
						if (j < 0) {
							if (errno != EWOULDBLOCK) 
								end_server = 1;
							break;
						}
						FD_SET (j,&master_set);
						if (j > max_sd)
							max_sd = j;
					} while (j != -1);
				} else {
					close_conn = 0;
					do {
						buffer = calloc (1,256 * sizeof (char));
						rc = recv(i,buffer,256 * sizeof (char),0);
						if (rc < 0) {
							if (errno != EWOULDBLOCK) {
								close_conn = 1;
							}
							break;
						}
						if (rc == 0) {
							close_conn = 1;
							break;
						}
						if (queue_connection (x++,i,buffer) < 0) {
							fprintf (stderr,"unable to place job in queue");
							break;
						}
					} while (1);
					if (close_conn) {
						close (i);
						FD_CLR (i,&master_set);
						if (i == max_sd) {
							while (FD_ISSET (max_sd,&master_set) == 0)
								max_sd -= 1;
						}
					}
				}
			}
		}
	} while (end_server == 0);
	
	for (i=0;i<=max_sd;i++) {
		if (FD_ISSET (i,&master_set))
			close (i);
	}

sprintf (msg,"thread: %d %s stop TID: %ld",mynode->threadnum,mynode->t_desc,(long int)mynode->t_id);
	message_send (msg,mynode->msg_qid);
	
	pthread_mutex_lock(&cq.control.mutex);
	queue_put(&cq.cleanup,(node *) mynode);
	pthread_mutex_unlock(&cq.control.mutex);
	pthread_cond_signal(&cq.control.cond);
	printf("thread %ld shutting down...\n",mynode->t_id);
	
	return NULL;
}

#define NUM_WORKERS 15

int numthreads;

void join_threads(void /**myargs*/) {
	/*cnode *mynode;
	mynode=(cnode *) myargs;
	mynode->t_id=syscall(224);
	mynode->t_desc="join threads";

	char *msg=(char *) malloc (sizeof (MSGSZ));
	sprintf (msg,"resonder stopping TID: %ld",(long int)mynode->t_id);
	*/
	cnode *curnode;

	printf("joining threads...\n");
	while (numthreads) {
		pthread_mutex_lock(&cq.control.mutex);
		/* below, we sleep until there really is a new cleanup node.  This
			takes care of any false wakeups... even if we break out of
			pthread_cond_wait(), we don't make any assumptions that the
			condition we were waiting for is true.  */
		while (cq.cleanup.head==NULL) {
		pthread_cond_wait(&cq.control.cond,&cq.control.mutex);
		}
		/* at this point, we hold the mutex and there is an item in the
			list that we need to process.  First, we remove the node from
			the queue.  Then, we call pthread_join() on the tid stored in
			the node.  When pthread_join() returns, we have cleaned up
			after a thread.  Only then do we free() the node, decrement the
			number of additional threads we need to wait for and repeat the
			entire process, if necessary */
		curnode = (cnode *) queue_get(&cq.cleanup);
		printf ("got curnode from cq");
		pthread_mutex_unlock(&cq.control.mutex);
		pthread_join(curnode->tid,NULL);
		printf(" joined with thread id: %ld\n",curnode->t_id);
		free(curnode);
		numthreads--;
	}
	//message_send (msg,mynode->msg_qid);
	
/*	pthread_mutex_lock(&cq.control.mutex);
	queue_put(&cq.cleanup,(node *) mynode);
	pthread_mutex_unlock(&cq.control.mutex);
	pthread_cond_signal(&cq.control.cond);
	
	return NULL;*/
}

int create_threads(void) {
	int x;
	int msg_id;
	cnode *curnode;
	end_server=0;
	end_logger=0;

	/*create message queue. */
	msg_id=message_init();
	
	curnode=malloc (sizeof(cnode));
	if (!curnode)
		return 1;
	curnode->threadnum=++numthreads;
	curnode->msg_qid=msg_id;
	if (pthread_create(&curnode->tid, NULL, logger, (void *) curnode))
		return 1;
	printf("created thread %d\n",curnode->threadnum);
	
	curnode=malloc (sizeof(cnode));
	if (!curnode)
		return 1;
	curnode->threadnum=++numthreads;
	curnode->msg_qid=msg_id;
	if (pthread_create(&curnode->tid, NULL, accepter, (void *) curnode))
		return 1;
	printf("created thread %d\n",curnode->threadnum);

	curnode=malloc(sizeof(cnode));
	if (!curnode)
		return 1;
	curnode->threadnum=++numthreads;
	curnode->msg_qid=msg_id;
	if (pthread_create(&curnode->tid, NULL, responder, (void *) curnode))
		return 1;
	printf("created thread %d\n",curnode->threadnum);

	return 0;
}

/*int stop_threads(void) {
	
	while (cq.cleanup.head!=NULL) {
	cq.clenaup.end_server=1;
	cq}
	if (!curnode)
		return 1;
	curnode->threadnum=0;
	if (pthread_create(&curnode->tid, NULL, logger, (void *) curnode))
		return 1;
	printf("created thread %d\n",0);
	numthreads=1;
	for (x=1; x<NUM_WORKERS; x++) {
		curnode=malloc(sizeof(cnode));
		if (!curnode)
			return 1;
		curnode->threadnum=x;
		if (pthread_create(&curnode->tid, NULL, producer, (void *) curnode))
			return 1;
		numthreads++;
		printf("created thread %d\n",x);
	}
	return 0;
}*/

void initialize_structs(void) {
	numthreads=0;
	if (control_init(&wq.control))
		dabort();
	queue_init(&wq.work);
	if (control_init(&cq.control)) {
		control_destroy(&wq.control);
		dabort();
	}
	queue_init(&wq.work);
	control_activate(&cq.control);
	control_activate(&wq.control);
}

void cleanup_structs(void) {
	control_destroy(&cq.control);
	control_destroy(&wq.control);
	printf ("control structures destroyed.\n");
}

int main(void) {

/*	if (signal(SIGHUP, mysighup) == SIG_ERR)
		printf("no handle for SIGHUP");
*/
	initialize_structs();
	/* CREATION */

	/* NETWORKING */

	if (create_threads()) {
		printf("Error starting threads... cleaning up.\n");
		//join_threads();
		dabort();
	}
	sleep(5);
	end_server = 1;
	sleep(10);
	printf("sleeping...\n");
	//sleep(5);
	join_threads();
	sleep (3);
	printf("deactivating work queue...\n");
	//control_deactivate(&wq.control);
	/* CLEANUP  */
//	end_logger=1;
	cleanup_structs();
	printf("done");
	exit (0);
}