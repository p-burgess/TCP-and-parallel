#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

// function prototypes
void parent_process(void);
void child_process1(char *, char *);
void child_process2(char *, char *, char *);
void child_process3(char *, char *, char *, char*);
void child_process4(char *, char *, char *, char *, char *);

int main(int argc, char **argv)
{
	pid_t pid;
	char *a, *b, *c, *d, *e;
	
	a = argv[1];
	b = argv[2];
	c = argv[3];
	d = argv[4];
	e = argv[5];
	
	switch(pid=fork()){
	case -1:
		fprintf(stderr, "fork failed");
		break;
	case 0:
		// child process
		//if (argc == 3){
			child_process1(a, b);
		//} else if (argc == 4){
		//	child_process2(a, b, c);
		//} else if (argc == 5){
		//	child_process3(a, b, c, d);
		//} else{
		//	fprintf(stderr, "incorrect number of ");
		//	fprintf(stderr, "arguements...\nexiting...\n");
		//	exit(1);
		//}
		break;
	default:
		// parent process
		parent_process();
	}
	return EXIT_SUCCESS;
}

void child_process1(char *a, char *b)
{
	char c[10] = "/bin/";
	strcat(c, a);
	printf("child process starting...\n");
	execl(c, a, b, 0);
	fprintf(stderr, "exec failed...");
	printf("child process finished");
}

void child_process2(char *a, char *b, char *d)
{
	char c[10] = "/bin/";
	strcat(c, a);
	printf("child process starting...\n");
	execl(c, a, b, d, 0);
	fprintf(stderr, "exec failed...");
	printf("child process finished");
}

void child_process3(char *a, char *b, char *d, char *e)
{
	char c[10] = "/bin/";
	strcat(c, a);
	printf("child process starting...\n");
	execl(c, a, b, d, e, 0);
	fprintf(stderr, "exec failed...");
	printf("child process finished");
}

void child_process4(char *a, char *b, char *d, char *e, char *f)
{
	char c[10] = "/bin/";
	strcat(c, a);
	printf("child process starting...\n");
	execl(c, a, b, d, e, f, 0);
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
