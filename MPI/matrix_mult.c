#include "mpi.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include<string>
using namespace std;
     int NRA ;      /* number of rows in matrix A */
     int NCA ;      /* number of columns in matrix A */
     int NCB ;     /* number of columns in matrix B */
 
#define MASTER 0        /* taskid of first task */
#define FROM_MASTER 1       /* setting a message type */
#define FROM_WORKER 2       /* setting a message type */
MPI_Status status;
void resalt_matrix(double **&array_ptr,int NRA)
{
                array_ptr=new double*[NRA]; //creates a new array of pointers to int objects
                for(int i=0; i<NRA; ++i)
                array_ptr[i]=new double[NRA];
}
void reading (string path,double **&array_ptr,int& NRA,int & NCA)
{
    ifstream inFile;
    double x=0;
    int size=0;
    bool var=true;
    int c1=0,c2=0,count=0;
    //double **array_ptr;   //two * are needed because it is a pointer to a point
    inFile.open(path);
    if (!inFile) 
    {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }
    else
    {
        while (inFile >> x) 
        {
            if(var==true)
            {
                size= x;
                NRA=size;
                NCA=size;
                NCB=size;
                size=0;
                array_ptr=new double*[NRA]; //creates a new array of pointers to int objects
                for(int i=0; i<NRA; i++)
                array_ptr[i]=new double[NRA];
                var=false;
                break;
                 
            }
             
        }
         
        while(inFile >> x)
        {
            /*if(var==false)
            {
                var==true;  
            }
            else
            {*/
            array_ptr[ c1 ][ c2 ]=x;
               if( inFile ) 
               {
                            c2=c2+1;
                            if( c2 == NRA ) 
                            {                            
                                ++c1;                   // Move to next row
                                c2 = 0;              
                            }
               }
//}
            }
        }
        inFile.close();
        for(int i=0;i<NRA;i++)
        {
            cout<<endl;
            for(int j=0;j<NRA;j++)
                cout<<array_ptr[i][j]<<"    ";
        }
        cout<<endl;
     
 
}
 
void main(int argc, char **argv) 
{
int numtasks,           /* number of tasks in partition */
    myid,           /* a task identifier */
    numworkers,         /* number of worker tasks */
    source,         /* task id of message source */
    dest,           /* task id of message destination */
    nbytes,         /* number of bytes in message */
    mtype,          /* message type */
    intsize,            /* size of an integer in bytes */
    dbsize,         /* size of a double float in bytes */
    rows,                       /* rows of matrix A sent to each worker */
    averow, extra, offset,      /* used to determine rows sent to each worker */
    i, j, k,            /* misc */
    count;
double **a=0;       /* matrix A to be multiplied */
double **b=0;       /* matrix B to be multiplied */
double **c=0;        /* result matrix C */
 
 
intsize = sizeof(int);
dbsize = sizeof(double);
 
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &myid);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
numworkers = 4;
 
/**************************** master task ************************************/
if (myid == MASTER) 
{
reading("E:\\ahmed.txt",a,NRA,NCA);
reading("E:\\matrix.txt",b,NCA,NCB);
resalt_matrix(c,NCA);
  printf("Number of worker tasks = %d\n",numworkers);
   for (i=0; i<NRA; i++)
   { 
    printf("\n"); 
    for (j=0; j<NCB; j++) 
      printf("%f   ", a[i][j]);
    }
  printf ("\n");
  for (i=0; i<NRA; i++) { 
    printf("\n"); 
    for (j=0; j<NCB; j++) 
      printf("%f   ", b[i][j]);
    }
  printf ("\n");
 
  /* send matrix data to the worker tasks */
  averow = NRA/numworkers;
  extra = NRA%numworkers;
  offset = 0;
  mtype = FROM_MASTER;
  for (dest=1; dest<=numworkers; dest++) {           
    rows = (dest <= extra) ? averow+1 : averow;      
    printf("   sending %d rows to task %d\n",rows,dest);
    MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
    count = rows*NCA;
    MPI_Send(&a[offset][0], count, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
    //MPI::COMM_WORLD.Send(&a[offset][0],count,MPI::DOUBLE,dest,0);
    count = NCA*NCB;
    MPI_Send(&b, count, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
 
    offset = offset + rows;
    }
 
  /* wait for results from all worker tasks */
  mtype = FROM_WORKER;
  for (i=1; i<=numworkers; i++)  {           
    source = i;
    MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
    MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
    count = rows*NCB;
    MPI_Recv(&c[offset][0], count, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, 
               &status);
  
    }
 
  /* print results */
  printf("Here is the result matrix\n");
  for (i=0; i<NRA; i++) { 
    printf("\n"); 
    for (j=0; j<NCB; j++) 
      printf("%f   ", c[i][j]);
    }
  printf ("\n");
 
  }  /* end of master section */
 
 
 
/**************************** worker task ************************************/
if (myid > MASTER) {
  mtype = FROM_MASTER;
  source = MASTER;
  printf ("Master =%d, mtype=%d\n", source, mtype);
  MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
  printf ("offset =%d\n", offset);
  MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
  printf ("row =%d\n", rows);
  count = rows*NCA;
  MPI_Recv(&a, count, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
  printf ("a[0][0] =%e\n", a[0][0]);
  count = NCA*NCB;
  MPI_Recv(&b, count, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
  printf ("b=\n");
  for (k=0; k<NCB; k++)
    for (i=0; i<rows; i++) {
      c[i][k] = 0.0;
      for (j=0; j<NCA; j++)
        c[i][k] = c[i][k] + a[i][j] * b[j][k];
      }
 
  mtype = FROM_WORKER;
  printf ("after computer\n");
  MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
  MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
  MPI_Send(&c, rows*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
  printf ("after send\n");
 
  }  /* end of worker */
  MPI_Finalize();
} /* of main */