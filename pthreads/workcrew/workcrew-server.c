#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "control.c"
#include "queue.c"
#include "dbug.h"
#include "message.c"

#include "workcrew.h"

#define NUM_PRODUCER 4
#define NUM_CONSUMER 4
#define NUM_RESPONDER 4

#define PORT    5555
#define MAXMSG  512

int numconnects;

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
  struct sigaction *sig_act;
} cnode;

    
int  read_from_client (int filedes) {
	char buffer[MAXMSG];
	int nbytes;

	nbytes = read (filedes, buffer, MAXMSG);
	if (nbytes < 0)	{
		/* Read error. */
		perror ("read");
		exit (EXIT_FAILURE);
	}
	else if (nbytes == 0)
		/* End-of-file. */
		return -1;
	else {
		/* Data read. */
		fprintf (stderr, "Server: got message: `%s'\n", buffer);
		return 0;
	}
}

/**
an array of cnode's needed to identify the logger-thread_id[0], 
and the signalhandler-thread_id[1].
**/
cnode *thread_id[2];

int stop_server (void);
int stop_thread (cnode *mynode);
void *signalhandler (void *myarg) {
	cnode *mynode;
	mynode = (cnode *)myarg;
	mynode->tid = pthread_self ();
	
	char *msg;
	msg = (char *) calloc (1,MSGSZ * sizeof (char));

	sigaddset (&mynode->sig_act->sa_mask,SIGCHLD);
	sigaddset (&mynode->sig_act->sa_mask,SIGUSR1);
	sigaddset (&mynode->sig_act->sa_mask,SIGUSR2);
	pthread_sigmask (SIG_SETMASK,&mynode->sig_act->sa_mask,NULL);

	pid_t pid_child;
	int status,sig,x;

	printf ("thread: %d signalhandler start TID: %u\n",mynode->threadnum,(unsigned int)mynode->tid);

	while (1) {
		if (!sigwait(&mynode->sig_act->sa_mask,&sig)) {
			switch (sig) {
				case SIGUSR1 : {
					sprintf (msg,"Thread-%d:%u caught signal: %d",mynode->threadnum,(unsigned int)mynode->tid,sig);
					message_send (msg,mynode->msg_qid);
					stop_server ();
					/*stop_logger (1);
					sleep (5);
					x = start_logger ();
					sleep (2);
					mynode->msg_qid = x;
					start_server (x);*/
					break;
					}
				case SIGUSR2 : {
					sprintf (msg,"Thread-%d:%u caught signal: %d",mynode->threadnum,(unsigned int)mynode->tid,sig);
					message_send (msg,mynode->msg_qid);
					stop_server ();
					sleep (4);
					printf ("%u:here.\n",(unsigned int)mynode->msg_qid);
//					x = start_logger ();
//					mynode->msg_qid = x;
					start_server (mynode->msg_qid);
					//start_server (mynode->msg_qid);
					break;
					}
				case SIGCHLD : { 	/* clean up zombie processes. */
					while ((pid_child = waitpid (-1,&status,WNOHANG)) > 0);
					sprintf (msg,"Thread-%d:%u caught signal: %d",mynode->threadnum,(unsigned int)mynode->tid,sig);
					message_send (msg,mynode->msg_qid);
					sprintf (msg,"Child: %d exited with status: %d",pid_child,status);
					message_send (msg,mynode->msg_qid);
					break;
				}
				default : {
					sprintf (msg,"Thread-%d:%u caught signal: %d",mynode->threadnum,(unsigned int)pthread_self (),sig);
					message_send (msg,mynode->msg_qid);
					break;
				}
			}
		}
	}

	stop_thread (mynode);
	return NULL;
}
void *logger(void *myarg) {
	cnode *mynode;
	mynode=(cnode *) myarg;
	mynode->tid = pthread_self ();
//	mynode->sig_act->sa_handler = (void *)stop_logger;

//	sigaddset (&mynode->sig_act->sa_mask,SIGUSR2);
//	pthread_sigmask (SIG_SETMASK,&mynode->sig_act->sa_mask,NULL);
	
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
	
	char *msg;
	msg = (char *) calloc (1,MSGSZ * sizeof (char));
	while (1) {
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
	// close message queue
	stop_thread (mynode);
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
		//we got something!
		myresponse=(wnode *) queue_get(&rq.work);
		myresponse->responder=pthread_self();
		pthread_mutex_unlock(&rq.control.mutex);
		sprintf (msg,"Thread-%d:%u responding job: %d from: %u",mynode->threadnum,(unsigned int)myresponse->responder,myresponse->job,(unsigned int)myresponse->processor);
		message_send (msg,mynode->msg_qid);
		free(myresponse);
		pthread_mutex_lock(&rq.control.mutex);
	}
	pthread_mutex_unlock(&rq.control.mutex);

	stop_thread (mynode);
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
	while (rq.control.active) {
		while (aq.work.head==NULL && aq.control.active) {
			pthread_cond_wait(&aq.control.cond, &aq.control.mutex);
		}
		if (!aq.control.active) 
			break;
		//we got something!
		myprocess=(wnode *) queue_get(&aq.work);
		myprocess->processor=pthread_self();
		pthread_mutex_unlock(&aq.control.mutex);
		//process job
		pthread_mutex_lock(&rq.control.mutex);
		queue_put(&rq.work,(node *) myprocess);
		pthread_mutex_unlock(&rq.control.mutex);
		pthread_cond_signal(&rq.control.cond);
		sprintf (msg,"Thread-%d:%u processed job: %d from: %u",mynode->threadnum,(unsigned int)myprocess->processor,myprocess->job,(unsigned int)myprocess->acceptor);
		message_send (msg,mynode->msg_qid);
		pthread_mutex_lock(&aq.control.mutex);
	}
	pthread_mutex_unlock(&aq.control.mutex);
	pthread_mutex_unlock(&rq.control.mutex);

	stop_thread (mynode);
	return NULL;
}
void *acceptor(void *myarg) {
	wnode *myconnect;
	cnode *mynode;
	mynode=(cnode *) myarg;
	int x;
	struct timespec time;
	time.tv_sec = 0;			
	time.tv_nsec = 27500000;
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
			myconnect->job=numconnects++;
			myconnect->acceptor=pthread_self();
			queue_put(&aq.work,(node *) myconnect);
			pthread_mutex_unlock(&aq.control.mutex);
			pthread_cond_signal(&aq.control.cond);
			sprintf (msg,"Thread-%d:%u produced job: %d",mynode->threadnum,(unsigned int)myconnect->acceptor,myconnect->job);
			nanosleep (&time,NULL);
			message_send (msg,mynode->msg_qid);
//			printf ("Thread-%d:%u produced job: %d\n",mynode->threadnum,(unsigned int)myconnect->acceptor,myconnect->job);
			sched_yield ();
			nanosleep (&time,NULL);
			pthread_mutex_lock(&aq.control.mutex);
		}
	}
	pthread_mutex_unlock(&aq.control.mutex);

	stop_thread (mynode);
	return NULL;
}
int numthreads;
int join_threads(void) {
  cnode *curnode;
  int num_threads=numthreads;
  printf("joining threads...\n");
  numthreads--;
  while (numthreads+2) {
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
  sleep (1);
  return num_threads;
}
int create_threads(int msg_qu_id) {
	int x;
	cnode *curnode;
	for (x=0; x<NUM_RESPONDER; x++) {
		curnode=malloc(sizeof(cnode));
		if (!curnode)
			return 1;
		curnode->threadnum=numthreads;
		curnode->msg_qid = msg_qu_id;
		if (pthread_create(&curnode->tid, NULL, responder, (void *) curnode))
			return 1;
		curnode->sig_act = (struct sigaction *) malloc (sizeof (struct sigaction));
		if (curnode->sig_act==NULL)
			return 1;
		printf("created responder: %d\tTID:%u\n",curnode->threadnum,(unsigned int)curnode->tid);
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
		curnode->sig_act = (struct sigaction *) malloc (sizeof (struct sigaction));
		if (curnode->sig_act==NULL)
			return 1;
		printf("created acceptor: %d\tTID:%u\n",curnode->threadnum,(unsigned int)curnode->tid);
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
		curnode->sig_act = (struct sigaction *) malloc (sizeof (struct sigaction));
		if (curnode->sig_act==NULL)
			return 1;
		printf("created processor: %d\tTID:%u\n",curnode->threadnum,(unsigned int)curnode->tid);
		numthreads++;
	}
	return 0;
}
int start_logger (void) {
	int msg_queue_id = message_init();
	cnode *curnode;
	curnode = (cnode *) malloc (sizeof (cnode));
	thread_id[0] = curnode;
	if (!curnode)
		return 1;
	curnode->threadnum=numthreads;
	curnode->msg_qid = msg_queue_id;
	if (pthread_create(&curnode->tid, NULL, logger, (void *) curnode))
		return 1;
	curnode->sig_act = (struct sigaction *) malloc (sizeof (struct sigaction));
	if (curnode->sig_act==NULL)
		return 1;
	numthreads++;
	return msg_queue_id;
}
int stop_logger (int signo) {
	cnode *curnode;
	curnode = thread_id[0];
	msgctl (curnode->msg_qid,IPC_RMID,NULL);
	stop_thread (curnode);
	pthread_kill (SIGTERM,curnode->tid);
	numthreads--;
	return 0;
}
int start_signalhandler (int msg_queue_id) {
	cnode *curnode;
	curnode = (cnode *) malloc (sizeof (cnode));
	thread_id[1] = curnode;
	if (!curnode)
		return 1;
	curnode->threadnum=numthreads;
	curnode->msg_qid = msg_queue_id;
	if (pthread_create(&curnode->tid, NULL, signalhandler, (void *) curnode))
		return 1;
	curnode->sig_act = (struct sigaction *) malloc (sizeof (struct sigaction));
	if (curnode->sig_act==NULL)
		return 1;
	numthreads++;
	return 0;
}
int stop_signalhandler (void) {
	cnode *curnode;
	curnode = thread_id[1];

	stop_thread (curnode);
	numthreads--;
	exit (1);//pthread_kill (SIGTERM,curnode->tid);
}
int stop_thread (cnode *mynode) {
	pthread_mutex_lock(&cq.control.mutex);
	queue_put(&cq.cleanup,(node *) mynode);
	pthread_mutex_unlock(&cq.control.mutex);
	pthread_cond_signal(&cq.control.cond);
	printf("thread %d shutting down...\n",mynode->threadnum);
	return 0;
 }
void initialize_structs(void) {
	numthreads=0;
	numconnects=0;
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
	printf("deactivating accept queue...\n");
	control_deactivate(&aq.control);
	sleep (2); // process outstanding connections.
	printf("deactivating respond queue...\n");
	control_deactivate(&rq.control);
	sleep (2); // respond to outstanding connections.
	printf("deactivating thread control queue...\n");
	control_deactivate(&cq.control);
	/* CLEANUP  */
	x = join_threads(); // =acceptors+processors+responders
	sleep (1);
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
	return 0;
}
int main(void) {
	/* signal handling: 
		block signals to all processes. */
	struct sigaction sa;

//	sigfillset (&sa.sa_mask);
	sigaddset (&sa.sa_mask,SIGUSR1);
	sigaddset (&sa.sa_mask,SIGUSR2);
	sigaddset (&sa.sa_mask,SIGCHLD);
	pthread_sigmask (SIG_BLOCK, &sa.sa_mask, NULL);

	int ipc;
	ipc  = start_logger ();
	start_signalhandler (ipc);
	start_server (ipc);

	extern int make_socket (uint16_t port);
	int sock;
	fd_set active_fd_set, read_fd_set;
	int i;
	struct sockaddr_in clientname;
	size_t size;

	/* Create the socket and set it up to accept connections. */
	sock = make_socket (PORT);
	if (listen (sock, 1) < 0) {
	   perror ("listen");
	   exit (EXIT_FAILURE);
	}

	/* Initialize the set of active sockets. */
	FD_ZERO (&active_fd_set);
	FD_SET (sock, &active_fd_set);

	while (1) {
	   /* Block until input arrives on one or more active sockets. */
	   read_fd_set = active_fd_set;
	   if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
	       perror ("select");
	       exit (EXIT_FAILURE);
	   }

	   /* Service all the sockets with input pending. */
	   for (i = 0; i < FD_SETSIZE; ++i) {
		if (FD_ISSET (i, &read_fd_set)) {
			 if (i == sock) {
				/* Connection request on original socket. */
				int new;
				size = sizeof (clientname);
				new = accept (sock, (struct sockaddr *) &clientname, &size);
				if (new < 0) {
					perror ("accept");
					exit (EXIT_FAILURE);
				}
				fprintf (stderr,
				"Server: connect from host %s, port %hd.\n",
				inet_ntoa (clientname.sin_addr),
				ntohs (clientname.sin_port));
				FD_SET (new, &active_fd_set);
			}
			else {
				/* Data arriving on an already-connected socket. */
				if (read_from_client (i) < 0) {
					close (i);
					FD_CLR (i, &active_fd_set);
				}
			}
		}
	}

	printf("sleeping...\n");
	sleep(2);
//	stop_server ();
	kill(getpid(),SIGUSR1);
	sleep (3);
	start_server (ipc);
	sleep (2);
	printf ("all threads ended.\n");
	exit (0);
}

     

 
