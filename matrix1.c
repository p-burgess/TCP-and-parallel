#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <stdio.h>
#include <stdlib.h>

#define n 400

/* Program to mulitply two 500 x 500 matricies */

main (void)
{
    int mat1[n][n], mat2[n][n];
    int result[n][n];
    int i, j, k;
    int m1 =n, n1=n, m2=n, n2=n;
        
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
