#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

// function prototypes
void parent_process(void);
void child_process(char *, char *);

int main(int argc, char **argv)
{
	pid_t pid;
	char *a, *b;
	
	a = argv[1];
	b = argv[2];
	
	switch(pid=fork()){
	case -1:
		fprintf(stderr, "fork failed");
		break;
	case 0:
		// child process
		child_process(a, b);
		break;
	default:
		// parent process
		parent_process();
	}
	return EXIT_SUCCESS;
}

void child_process(char *a, char *b)
{
	char c[10] = "/bin/";
	strcat(c, a);
	printf("child process starting...\n");
	execl(c, a, b, 0);
	fprintf(stderr, "exec failed...");
	printf("child process finished");
}

void parent_process(void)
{
	printf("parent waiting...\n");
	wait(NULL);
	printf("parent done.\n");
	exit(0);
}
