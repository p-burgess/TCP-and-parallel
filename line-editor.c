/* A simple line editor written by Paul Burgess 10079217
 * for course U08006 C and Unix
 */



// print instructions
//

#include <stdio.h>
#include <alloc.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

/* function prototypes. */
struct node * initnode(char *,int);
void printnode(struct node *);
void printlist
void skip_eol() /*skip past newline character in stdin */
{
	int ch;	/* use int for fgetc return */
	while((ch=fgetc(stdin))!=EOF && ch!='\n');
}
// enter a line
//
// use a switch case in a while loop to implement editing


int main()
{
	// Inintialise the variables.
	//
	//
	// This is a link in a doubly linked list.
	//
	typedef struct link{
		char letter;
		struct link *next;
		struct link *prev;
	} node;

	char aletter;
	node *alink;
	int *prev_ptr = NULL;
	int *next_ptr = NULL;

	printf("Line Editor\n\n");
	printf("enter a line of text :\n");
	// create the first link with a space in it


	skip_eol();
	do{
		alink = (node*) malloc (sizeof(node));
		aletter = getchar();
		alink->letter = aletter;
		alink->next = *next_ptr;
		alink->prev = *prev_ptr;
		*prev_ptr = alink;
	}while(aletter!='\n' && aletter!='x' && aletter!='X')
}



