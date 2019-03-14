/* Paul Burgess 
 * 10079217
 *
 * Question 1
 * A program to open a text file "my.txt"
 * for opening a text file.
 */
 
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#define MaxPWDLen 30

// global variables
char port[16]="";
char root_dir[128]="";
char cig_dir[128]="";


// function prototypes
int check_access(char *filename);
int print_prompt(void);
void read_file(int fd);
void *read_line(char line);
int check_password(void);
void skip_eol(void);
char* filter(char* src_string, char* dest_string);
void append(char* s, char c);

// subroutines

/* skip past newline in keyboard input stream. */
void skip_eol(void)		
{ 
	int ch;
	while ((ch=fgetc(stdin))!=EOF && ch!='\n');
}


int print_prompt(void)
{
	// initialize variables
	int config_fd;
	char config_file[80];
	
	// prompt for filename
	printf("\nEnter the configuration file name: ");
	scanf("%s", config_file);		
	
	skip_eol();
	
	// check access
	// 		ok	- return filename
	// 		not - return "" :(empty string)
	while ((config_fd = check_access(config_file)) == -1 )
	{
		printf("please re-enter filename : ");
		scanf("%s", config_file);
	}
	
	return config_fd;
}
		
int check_access(char *filename)
{
	// check permission
	// 		bad  	- return 0
	// 		good 	- return file descriptor
	if (access(filename, R_OK) == -1)
	{
		fprintf(stderr, "\n\nThe file %s does ", filename);
		fprintf(stderr, "not exist or, \nyou do not have");
		fprintf(stderr, "the  necessary permissions... \n");
		return -1;
	}
	else
	{
		printf("reading configuration file: %s\n\n", filename);
		return open(filename, O_RDONLY);
	}
}


void read_file(int config_fd)
{
	// initialize variables
	char c;
	char *d;
	char e[256];
	char line [ 128 ];
	while(scanf("%s",line)/*, sizeof line, (FILE *)config_fd)*/ != NULL)/* read a line */{ 
	

	printf("%s\n",line);

		}

	// search the string for delimiter
	// divide string into username and password
	// add mnode to end of list
	// malloc userID node
	
	
	
	// read more lines - create user database
	
	strcpy(d,""); 
	ssize_t nread;

		// loop until return value 0 = EOF
		while ((nread = read(config_fd, &c, 1)) != 0)
		{
			append(d, c);
		}
		//puts(filter(d,e));
}

	
/* appends a character to the end of a string. */
void append(char* s, char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}


int main(int argc, char *argv[])
{
	// intialize variables
	int config_fd;
	
	if (argc != 2)
	{
		// prompt for filename
		// if filename = "" - reprompt
		config_fd = print_prompt();
		read_file(config_fd);
	}
	else
	{
		// check access
		// print file
		//print_file(check_access(argv[1]));
	}
	
	// close the file
	close(config_fd);
	return EXIT_SUCCESS;
	
}




