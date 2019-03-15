/* R Yusoof 7.2.11 A1 Q3 */
#include <stdio.h>
#include <stdlib.h>

//This programme allows the user to calculate the area of a square, rectangle and circle

void skip_eol()/* skip past newline in keyboard input stream */
{ int ch;
  while ((ch=fgetc(stdin))!=EOF && ch!='\n');
}


int main()


{ int Rlength;  /* rectangle length */
  int Rwidth;   /* rectangle width */
  int Rarea;    /* calculated rectangle area */
  int Slength;	/* square length */
  int Swidth;	/* square width */
  int Sarea;	/* calculated square area */
  int Cradius;	/* circle radius */
  int Carea;	/* calculated circle area */
  char shape;	/* desired shape */
 
  int pi = 3.14;
  
  

  printf("Area Calculator\n");
  printf("Enter shape (S=square, R=rectangle, C=circle):  ");
  scanf("%c", &shape); /* input desired shape */
  
  if(shape == 'S')
	{printf("Enter square length and width  ");
	scanf("%d %d", &Slength, &Swidth); /* input rectangle dimensions */
		Sarea = Slength*Swidth;
			printf("\nSquare area = %d\n", Sarea);
			}
  	if(shape == 'R')
		{printf("Enter rectangle length and width  ");
		scanf("%d %d", &Rlength, &Rwidth); /* input rectangle dimensions */
		Rarea = Rlength*Rwidth;
			printf("\nSquare area = %d\n", Rarea);	
			}
		if(shape == 'C')
			{printf("Enter circle radius  ");
			scanf("%d", &Cradius); /* input rectangle dimensions */
		Carea = pi*Cradius*Cradius;
			printf("\nCircle area = %d\n", Carea);
			}		
				else;
  return 0;
}



