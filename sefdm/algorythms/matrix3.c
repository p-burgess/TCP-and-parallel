  #include <sys/types.h>
  #include <sys/wait.h>
  #include <unistd.h>
  #include <time.h>
  #include <sys/times.h>
  #include <stdio.h>
  #include <stdlib.h>

void child_process (void);
int fork_processes(int number_process);
  
main (void)
{
	int i, j, trial[]={1,2,3,4,5,10,15,20,25,30,35,40,50,75,100};
	
	double lastcuTime = 0.0, lastcsTime = 0.0;
	time_t currentTime;
    	struct tms cpuTime;

	printf("1 to 100 processes\n\n");
	printf("processes:user time:system time\n");
	fflush(stdout); 

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (fork_processes(trial[i]) == 1)
			   sleep(1);
		}

		times(&cpuTime);

		printf("%d:",	trial[i]);
		printf("%f:",	((double) (cpuTime.tms_cutime-lastcuTime)/(trial[i]*4)));
		printf("%f\n",((double) (cpuTime.tms_cstime-lastcsTime)/(trial[i]*4)));
		fflush(stdout);
	
		lastcuTime = cpuTime.tms_cutime;
		lastcsTime = cpuTime.tms_cstime;
	}
	for (i = 8; i < 15; i++)
	{
		if (fork_processes(trial[i])==1){
			sleep(1);

			times(&cpuTime);

			printf("%d:",	trial[i]);
			printf("%f:",	((double) (cpuTime.tms_cutime-lastcuTime)/trial[i]));
			printf("%f\n",((double) (cpuTime.tms_cstime-lastcsTime)/trial[i]));
			fflush(stdout);
	
			lastcuTime = cpuTime.tms_cutime;
			lastcsTime = cpuTime.tms_cstime;
		}
	}
}
  
int fork_processes(int number_process)
{
	pid_t pids[100],pid;
	int i, status;

	/* Start children. */
	for (i = 0; i < number_process; ++i) {	/* Start a child process.   */
		if ((pids[i] = fork()) == -1) {
			perror("fork error");
			exit(EXIT_FAILURE);
		} else if (pids[i] == 0) {	/* This is the child.       */
			child_process();
			exit(EXIT_SUCCESS);
		} else {                    /* This is the parent.      */
			
		}
	}
	
	/* Wait for children to exit. */
	for(i = 0; i < number_process; i++)
		while(0<waitpid(pids[i],NULL,0));

	return 1;
}

/* Program to mulitply two 500 x 500 matricies */

void child_process (void)
{
    int mat1[400][400], mat2[400][400];
    int result[400][400];
    int i, j, k;
    int m1 =400, n1=400, m2=400, n2=400;
	
	/* initialize matricies */
    for (i = 0; i < m1; i++)
    {
        for (j = 0; j < n1; j++)
        {
           mat1[i][j] = rand();
        }
    }
    
    for (i = 0; i < m1; i++)
    {
        for (j = 0; j < n1; j++)
        {
            mat2[i][j]= rand();
        }
    }
    
    /* now multiply matricies... */
    
    for (i = 0; i < m1; i++)
    {
        for (j = 0; j < n2; j++)
        {
            result[i][j] = 0;
            for (k = 0; k < m1; k++)
                result[i][j] += mat1[i][k] * mat2[k][j];
        }
    }
}
