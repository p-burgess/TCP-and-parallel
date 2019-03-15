#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{ 
	double usr_time, sys_time;
	int target, a, i, flag = 0; 
	
	clock_t tcstart, tcend;

	struct tms start, end;

	// start timer
	if ((tcstart=times(&start))==-1){
		printf("Error to get start time\n");
		return 1;
	}

	// routine to find prime numbers to command line arguement
	target=atoi(argv[1]);

	for (a = 2; a <= target ;a++){ 
		for (i = 2; i < a ;i++){
			if (a % i == 0)
			flag = 1;
		}
		if (flag == 0)
			{/*printf("%d\n",a);*/}
		flag = 0;
	}
	
	// end timer and print
	if ((tcend=times(&end))==-1){
		printf("Error to get final time\n");
		return 1;
	}

	printf("user time:\t%d\n", (end.tms_utime-start.tms_utime));
	printf("system time:\t%d\n", (end.tms_stime-start.tms_stime));

	return 0;
}
 
