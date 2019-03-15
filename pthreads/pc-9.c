/*
 *	File	: pc.c
 *
 *	Title	: Demo Producer/Consumer.
 *
 *	Short	: A solution to the producer consumer problem using
 *		pthreads.	
 *
 *	Long 	:
 *
 *	Author	: Andrae Muys, Paul Burgess
 *
 *	Date	: 18 September 1997
 *
 *	Revised	:
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define QUEUESIZE 10
#define LOOP 20

void *producer (void *args);
void *consumer (void *args);

typedef struct {
	int buf[QUEUESIZE];
	//char *buffer;
	long head, tail;
	int full, empty;
	pthread_mutex_t *mut;
	pthread_cond_t *notFull, *notEmpty;
} queue;

queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, int in/*, char *buffer*/);
void queueDel (queue *q, int *out);
void mysighup ();

int main ()
{
	queue *fifo;
	pthread_t pro, con;
//	char *prod="producer";

	if (signal(SIGHUP, mysighup) == SIG_ERR)
		printf("no handle for SIGHUP");

	fifo = queueInit ();
	if (fifo ==  NULL) {
		fprintf (stderr, "main: Queue Init failed.\n");
		exit (1);
	}
	pthread_create (&pro, NULL, producer, fifo);
	pthread_create (&con, NULL, consumer, fifo);
	pthread_join (pro, NULL);
	pthread_join (con, NULL);

	sleep (2);

	pthread_kill(pro,SIGTERM);
	pthread_kill(con,SIGTERM);

	queueDelete (fifo);

	printf("handle for SIGHUP");
	kill(getpid(),SIGHUP);

	return 0;


}

void *producer (void *q)
{
	queue *fifo;
	int i=0;
	//char *buffer="empty";

	fifo = (queue *)q;

	while (i<4) {
		pthread_mutex_lock (fifo->mut);
		while (fifo->full) {
			printf ("producer: queue FULL.\n");
			pthread_cond_wait (fifo->notFull, fifo->mut);
		}
		queueAdd (fifo, i++/*, buffer*/);
		pthread_mutex_unlock (fifo->mut);
		pthread_cond_signal (fifo->notEmpty);
		usleep (100000);
	}

	pthread_exit(0);

	return (NULL);
}

void *consumer (void *q)
{
	queue *fifo;
	int i, d;
	FILE *log_file;

	fifo = (queue *)q;

	log_file = fopen ("./log.text","a");

	while (1) {
		pthread_mutex_lock (fifo->mut);
		while (fifo->empty) {
			printf ("consumer: queue EMPTY.\n");
			pthread_cond_wait (fifo->notEmpty, fifo->mut);
		}
		queueDel (fifo, &d);
		pthread_cond_signal (fifo->notFull);
		pthread_mutex_unlock (fifo->mut);
		//pthread_cond_signal (fifo->notFull);
		printf ("consumer: recieved %d.\n", d);
//		fprintf(log_file,":%d:\n",d);
		usleep(200000);
	}

	return (NULL);
}

/*#ifdef 0
typedef struct {
	int buf[QUEUESIZE];
	long head, tail;
	int full, empty;
	pthread_mutex_t *mut;
	pthread_cond_t *notFull, *notEmpty;
} queue;
#endif*/

queue *queueInit (void)
{
	queue *q;

	q = (queue *)malloc (sizeof (queue));
	if (q == NULL) return (NULL);

	q->empty = 1;
	q->full = 0;
	q->head = 0;
	q->tail = 0;
	//q->buffer = (char *) malloc (15 * sizeof (char));
	q->mut = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
	pthread_mutex_init (q->mut, NULL);
	q->notFull = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
	pthread_cond_init (q->notFull, NULL);
	q->notEmpty = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
	pthread_cond_init (q->notEmpty, NULL);
	
	return (q);
}

void queueDelete (queue *q)
{
	pthread_mutex_destroy (q->mut);
	free (q->mut);	
	pthread_cond_destroy (q->notFull);
	free (q->notFull);
	pthread_cond_destroy (q->notEmpty);
	free (q->notEmpty);
	free (q);
}

void queueAdd (queue *q, int in/*, char *buffer*/)
{
	q->buf[q->tail] = in;
	//strcpy (q->buffer, buffer);

	q->tail++;
	if (q->tail == QUEUESIZE)
		q->tail = 0;
	if (q->tail == q->head)
		q->full = 1;
	q->empty = 0;

	return;
}

void queueDel (queue *q, int *out)
{
	*out = q->buf[q->head];

	q->head++;
	if (q->head == QUEUESIZE)
		q->head = 0;
	if (q->head == q->tail)
		q->empty = 1;
	q->full = 0;

	return;
}

void mysighup ()
{
	printf("caught SIGHUP\n");

	return;
}
