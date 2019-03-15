#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	// create text file with control characters
	
	// initialize variables
	int i;
	char c;
	int fd;
	
	//open file for writing
	fd = open("./test.txt",O_WRONLY,777);
	
	//write all characters to file
	for (i = 0; i<128;i++)
	{
		c = (char)i;
		write (fd,&c,1);
	}
	close(fd);
	return EXIT_SUCCESS;
}
