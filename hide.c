#include <stdio.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#include <malloc.h>
#include <string.h>

char buffer;
char sfn[50];
char tfn[50];
int sh;
int th;
int numread;


void encode()
{
	unsigned char temp, e, d = 0;

	while (numread = read(sh, &buffer, 1)){
		e = buffer;
		temp = (e >> 4) | d;
		d = buffer << 4;
		write(th, &temp, numread);		
	}
	write(th, &d, 1);
}

void decode()
{
	unsigned char temp, e, d = 0;

	numread = read(sh, &buffer, 1);
	d = buffer << 4;
	while (numread = read(sh, &buffer, 1)){
		e = buffer;
		temp = (e >> 4) | d;
		d = buffer << 4;
		write(th, &temp, numread);
	}
}

int main(int argc, char *argv[])
{

	if(argc < 3) return(1);

	strcpy(sfn, argv[1]);
	strcpy(tfn, argv[2]);

	if((sh = open(sfn, O_RDONLY | O_BINARY)) == -1){
		printf("not open r");
		return(1);
	}

	if((th = open(tfn, O_CREAT | O_WRONLY | O_BINARY/*, S_IWRITE*/)) == -1){
		printf("not open w");
		return(1);
	}

	if (argc == 4) {
		if (strcmp(argv[3],"u") == 0)
			decode();
		else 
			encode();
	}
	else 
		encode();

	close(sh);
	close(th);

	return(0);

}