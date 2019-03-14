/** From publib.boulder.ibm.com/infocenter/iseries/v6r1m0/index.jsp?topic=/rzahw/rzahwe19rx.htm 
**/
#include <sys/sem.h>
//#include "semaphore.h"
int semaphoreID=-1;
int numthreads=0;
struct sembuf lockOp = { 0, -1, 0};
struct sembuf unlockOp = { 0, 1, 0};

void initialize_Semaphore(void)
{
	semaphoreID = semget (IPC_PRIVATE, 1, S_IRUSR|S_IWUSR);
	if (semaphoreID < 0) {
		printf("semget failed, err=%d\n",errno);
		exit (1);
	}
	semctl(semaphoreID, 0, SETVAL, (int)1);
}
void incrementSemaphore (void) {
	semop (semaphoreID, &lockOp, 1);
	numthreads++;
	semop (semaphoreID, &unlockOp, 1);
}
void decrementSemaphore (void) {
	semop (semaphoreID, &lockOp, 1);
	numthreads--;
	semop (semaphoreID, &unlockOp, 1);
}
void destroySemaphore (void) {
	semctl (semaphoreID, 0, IPC_RMID);
}
