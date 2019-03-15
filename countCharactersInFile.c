#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUFSIZE 512

main()
{
	char buffer[BUFSIZE];
	int filedes;
	ssize_t nread;
	long total = 0;

	/* open "anotherfile" readonly*/
	if (( filedes = open("anotherfile", O_RDONLY)) == -1)
	{
		printf("error in opening anotherfile\n");
		exit(1);
	}

	/* loop until EOF, shown by return value of 0*/
	while( (nread = read(filedes, buffer, BUFSIZE)) > 0)
		total += nread;
	printf("total chars in anotherfile: %ld\n", total);
	exit(0);
}
