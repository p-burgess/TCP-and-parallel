#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "control.c"
#include "queue.c"
#include "dbug.h"
#include "message.c"

#define NUM_PRODUCER 4
#define NUM_CONSUMER 4
#define NUM_RESPONDER 4

int connection;

/* the work_queue holds tasks for the various threads to complete. */
struct work_queue {
  data_control control;
  queue work;
} aq;
/* the job_queue holds connection data that will be returned to the client. */
struct job_queue {
	data_control control;
	queue work;
} rq;
/* I added a job number to the work node.  Normally, the work node
   would contain additional data that needed to be processed. */
typedef struct work_node {
  struct node *next;
  int job;
  pthread_t responder;
  pthread_t processor;
  pthread_t acceptor;
} wnode;

/* I added a job number to the work node.  Normally, the work node
would contain additional data that needed to be processed. */
/*typedef struct job_node {
	struct node *next;
	int job;
	unsigned int worknum;
	unsigned int processor;
} jnode;*/
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
  struct node *next;
  int threadnum;
  pthread_t tid;
  int msg_qid;
} cnode;
void stop_thread (void *mynode);
void *logger(void *myarg) {
	cnode *mynode;
	mynode=(cnode *) myarg;
	char *msg;
	msg = (char *) calloc (1,MSGSZ * sizeof (char));
	time_t currentTime;
	struct tm * Time;

	printf ("thread: %d logger start TID: %u\n",mynode->threadnum,(unsigned int)pthread_self());

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

	while (aq.control.active || rq.control.active || cq.control.active) {
		message_recv (msg,mynode->msg_qid);//blocks for message
		currentTime=time (NULL);
		Time = localtime ( &currentTime );
		printf ("%d:",Time->tm_year+1900);
		printf ("%d:",Time->tm_mon+1);
		printf ("%d:",Time->tm_mday+1);
		printf ("%d:",Time->tm_hour);
		printf ("%d:",Time->tm_min);
		printf ("%d:",Time->tm_sec);
		printf ("%s\n",msg);
		//fprintf (log_fd,"%s",msg);
		//^why doesn't this work?
	}
	//	fclose (log_fd);
	sleep (1);
	return NULL;
}
void *signal_handler(void *myarg) {
	cnode *mynode;
	mynode=(cnode *) myarg;
	char *msg;
	msg = (char *) calloc (1,MSGSZ * sizeof (char));

	printf ("thread: %d signal handler start TID: %u\n",mynode->threadnum,(unsigned int)pthread_self());

	while (aq.control.active || rq.control.active || cq.control.active) {
		message_recv (msg,mynode->msg_qid);//blocks for message
	}
	sleep (1);
	return NULL;
}
void *responder (void *myarg) {
	sleep (1);
	wnode *myresponse;
	cnode *mynode;
	mynode=(cnode *) myarg;
	struct timespec time;
	time.tv_sec = 0;			
	time.tv_nsec = 70000;
	char *msg;
	msg = (char *) calloc (1,MSGSZ * sizeof (char));
	pthread_mutex_lock(&rq.control.mutex);
	while (rq.control.active) {
		while (rq.work.head==NULL && rq.control.active) {
			pthread_cond_wait(&rq.control.cond, &rq.control.mutex);
		}
		if (!rq.control.active) 
			break;
		myresponse=(wnode *) queue_get(&rq.work);
		myresponse->responder=pthread_self();
		pthread_mutex_unlock(&rq.control.mutex);
		sprintf (msg,"Thread-%d:%u responding job: %d from: %u",mynode->threadnum,(unsigned int)myresponse->responder,myresponse->job,(unsigned int)myresponse->processor);
//		nanosleep (&time,NULL);
		message_send (msg,mynode->msg_qid);
		free(myresponse);
		pthread_mutex_lock(&rq.control.mutex);
	}
	pthread_mutex_unlock(&rq.control.mutex);
	return NULL;
}
void *processor (void *myarg) {
	sleep (1);
	wnode *myprocess;
	cnode *mynode;
	mynode=(cnode *) myarg;
	struct timespec time;
	time.tv_sec = 0;			
	time.tv_nsec = 700;
	char *msg;
	msg = (char *) calloc (1,MSGSZ * sizeof (char));
	pthread_mutex_lock(&aq.control.mutex);
	while (aq.control.active) {
		while (aq.work.head==NULL && aq.control.active) {
			pthread_cond_wait(&aq.control.cond, &aq.control.mutex);
		}
		if (!aq.control.active) 
			break;
		myprocess=(wnode *) queue_get(&aq.work);
		myprocess->processor=pthread_self();
		pthread_mutex_unlock(&aq.control.mutex);
		//process job
		nanosleep (&time,NULL);
		pthread_mutex_lock(&rq.control.mutex);
		queue_put(&rq.work,(node *) myprocess);
		pthread_mutex_unlock(&rq.control.mutex);
		pthread_cond_signal(&rq.control.cond);
		sprintf (msg,"Thread-%d:%u processed job: %d from: %u",mynode->threadnum,(unsigned int)myprocess->processor,myprocess->job,(unsigned int)myprocess->acceptor);
//		nanosleep (&time,NULL);
		message_send (msg,mynode->msg_qid);
		pthread_mutex_lock(&aq.control.mutex);
	}
	pthread_mutex_unlock(&aq.control.mutex);
	pthread_mutex_unlock(&rq.control.mutex);
	return NULL;
}
void *acceptor(void *myarg) {
	wnode *myconnect;
	cnode *mynode;
	mynode=(cnode *) myarg;
	int x;
	struct timespec time;
	time.tv_sec = 0;			
	time.tv_nsec = 7500000;
	char *msg;
	msg = (char *) calloc (1,MSGSZ * sizeof (char));
	sleep (1);
	pthread_mutex_lock(&aq.control.mutex);
	while (aq.control.active) {
		for (x=0;x<1;x++) {
			myconnect=malloc(sizeof(wnode));
			if (!myconnect) {
				printf("ouch! can't malloc!\n");
				pthread_mutex_unlock(&aq.control.mutex);
				break;
			}
			myconnect->job=connection++;
			myconnect->acceptor=pthread_self();
			queue_put(&aq.work,(node *) myconnect);
			pthread_mutex_unlock(&aq.control.mutex);
			pthread_cond_signal(&aq.control.cond);
			sprintf (msg,"Thread-%d:%u produced job: %d",mynode->threadnum,(unsigned int)myconnect->acceptor,myconnect->job);
//			nanosleep (&time,NULL);
			message_send (msg,mynode->msg_qid);
			nanosleep (&time,NULL);
			pthread_mutex_lock(&aq.control.mutex);
		}
	}
	pthread_mutex_unlock(&aq.control.mutex);
	return NULL;
}
int numthreads;
int join_threads(void) {
	cnode *curnode;
	int num_threads=numthreads;
	printf("joining threads...\n");
	numthreads--;
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
		pthread_mutex_unlock(&cq.control.mutex);
		pthread_join(curnode->tid,(void *)curnode);
		printf("joined with thread %d\n",curnode->threadnum);
		free(curnode);
		numthreads--;
	}
	return num_threads;
}
int create_threads(int msg_qu_id) {
	int x;
	cnode *curnode;
	connection=0;
	for (x=0; x<NUM_RESPONDER; x++) {
		curnode=malloc(sizeof(cnode));
		if (!curnode)
			return 1;
		curnode->threadnum=numthreads;
		curnode->msg_qid = msg_qu_id;
		if (pthread_create(&curnode->tid, NULL, responder, (void *) curnode))
			return 1;
		printf("created responder %d\n",curnode->threadnum);
		numthreads++;
	}
	for (x=0; x<NUM_PRODUCER; x++) {
		curnode=malloc(sizeof(cnode));
		if (!curnode)
			return 1;
		curnode->threadnum=numthreads;
		curnode->msg_qid = msg_qu_id;
		if (pthread_create(&curnode->tid, NULL, acceptor, (void *) curnode))
			return 1;
		printf("created acceptor %d\n",curnode->threadnum);
		numthreads++;
  }
  for (x=0; x<NUM_CONSUMER; x++) {
		curnode=malloc(sizeof(cnode));
		if (!curnode)
			return 1;
		curnode->threadnum=numthreads;
		curnode->msg_qid = msg_qu_id;
		if (pthread_create(&curnode->tid, NULL, processor, (void *) curnode))
			return 1;
		printf("created processor %d\n",curnode->threadnum);
		numthreads++;
	}
	return 0;
}
void stop_thread (void *arg) {
	cnode *mynode = arg;
	pthread_mutex_lock(&cq.control.mutex);
	queue_put(&cq.cleanup,(node *) mynode);
	pthread_mutex_unlock(&cq.control.mutex);
	pthread_cond_signal(&cq.control.cond);
	printf("thread %d shutting down...\n",mynode->threadnum);
	return;
}
int start_logger (void) {
	int msg_queue_id = message_init();
	cnode *curnode;
	curnode = (cnode *) malloc (sizeof (cnode));
	if (!curnode)
		return 1;
	curnode->threadnum=numthreads;
	curnode->msg_qid = msg_queue_id;
	if (pthread_create(&curnode->tid, NULL, logger, (void *) curnode))
		return 1;
	printf("created logger %d\n",curnode->threadnum);
	return msg_queue_id;
}
int start_signalhandler (int msg_queue_id) {
	cnode *curnode;
	curnode = (cnode *) malloc (sizeof (cnode));
	if (!curnode)
		return 1;
	curnode->threadnum=numthreads;
	curnode->msg_qid = msg_queue_id;
	if (pthread_create(&curnode->tid, NULL, signal_handler, (void *) curnode))
		return 1;
	printf("created signal handler %d\n",curnode->threadnum);
	return 0;
}
void initialize_structs(void) {
	numthreads=0;
	if (control_init(&aq.control))
		dabort();
	queue_init(&aq.work);
	if (control_init(&rq.control)) {
		control_destroy(&aq.control);
		dabort();
	}
	queue_init(&rq.work);
	if (control_init(&cq.control)) {
		control_destroy(&aq.control);
		control_destroy(&rq.control);
		dabort();
	}
	queue_init(&cq.cleanup);
	control_activate(&rq.control);
	control_activate(&aq.control);
	control_activate(&cq.control);
}
void cleanup_structs(void) {
	control_destroy(&aq.control);
	control_destroy(&rq.control);
	control_destroy(&cq.control);
}
int stop_server (void) {
	int x;
	printf("deactivating queues...\n");
	control_deactivate(&aq.control);
	sleep (1); // process outstanding connections.
	control_deactivate(&rq.control);
	sleep (1); // respond to outstanding connections.
	control_deactivate(&cq.control);
	/* CLEANUP  */
	x = join_threads(); // =numthreads-logger
	cleanup_structs();
	return x;
}
int start_server (int msg_qu_id) {
	initialize_structs();
	/* CREATION */
	if (create_threads(msg_qu_id)) {
		printf("Error starting threads... cleaning up.\n");
		join_threads();
		dabort();
	}
	printf("sleeping...\n");
	return 0;
}
int main(void) {	
	int msg_qu_id;
	/**     signal **/
	sigset_t set;
	sigemptyset (&set);
	sigaddset(&set,SIG_USR1);
	pthread_sigmask (SIG_BLOCK,&set,0);
	/**    signal  **/
	msg_qu_id  = start_logger ();
	start_server (msg_qu_id);
	sleep(1);
	printf ("%d threads ended.\n",stop_server ());
	sleep (1);
	printf ("%d start server.\n",start_server (msg_qu_id));
	sleep (1);
	stop_server ();
	sleep (2);
	exit (0);
}