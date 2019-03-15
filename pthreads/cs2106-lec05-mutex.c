#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;

void *less(void *arg)
{
	int i;
	int *x = (int *)arg;
	for (i = 0; i < 9; i++) {
		pthread_mutex_lock(&mutex);
		*x = *x - 1;
		printf("%3d", *x);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void *more(void *arg)
{
	int i;
	int *x = (int *)arg;
	pthread_mutex_lock(&mutex);
	for (i = 0; i < 9; i++) {
		*x = *x + 1;
		printf("%3d", *x);
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main()
{
	pthread_t more_thread, less_thread;
	int x = 0;

	pthread_mutex_init(&mutex, NULL);
	
	// create two threads
	pthread_create(&more_thread, NULL, more, (void *)&x);	
	pthread_create(&less_thread, NULL, less, (void *)&x);	
	
	// wait until they are done
	pthread_join(more_thread, NULL);
	pthread_join(less_thread, NULL);

	printf("\n");
}
