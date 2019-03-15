/* Bubble Sort program edited by Paul Burgess sourced from
*  http://cprogramminglanguage.net/c-bubble-sort-source-code.asx
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Function that uses pointer swapping to swap two integer values. */
void swap(int *x,int *y)
{
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

void bublesort(int list[], int n)
{
	int i,j;
	for(i=0;i<(n-1);i++)
		for(j=0;j<(n-(i+1));j++)
			if(list[j] > list[j+1])
				swap(&list[j],&list[j+1]);
}
 
void printlist(int list[],int n)
{
	int i;
	for(i=0;i<n;i++)
	printf("%d\n",list[i]);
}
 
int main(int argc, char *argv[])
{
	const int MAX_ELEMENTS = 100;
	int list[MAX_ELEMENTS];

	int i = 0, n = 0;
	n = atoi(argv[1]);
	n = (n > MAX_ELEMENTS) ? MAX_ELEMENTS : n;

	const int m=10000;

	printf("command line argument: %d\n", n);
	
	//sscanf(argv[1],"%d,&n);

	// generate random numbers and fill them to the list
	srand(time(NULL));
	for(i=0; i < n; i++)
	{
		list[i] = rand();
		printf("random number %d is %d\n", i+1, list[i]);
	}

    
//	printf("The list before sorting is:\n");
//	printlist(list, n);
 
	// sort the list
	bublesort(list, n);

	// print the result
	printf("The list after sorting using bubble sorting algorithm:\n");
	printlist(list, n);

	printf("This Algorithym took %d milliseconds\n\n\n", clock());
	return EXIT_SUCCESS;
}
