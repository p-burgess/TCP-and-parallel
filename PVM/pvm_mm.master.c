/******************************************************************************
* PVM Matrix Multiply - C Version 
* Master Program
* FILE: pvm_mm.master.c
* OTHER FILES: pvm_mm.worker.c, make.mm.c
* DESCRIPTION:  PVM matrix multiply example code master task.  C version.
*   In this example code, the master program acts as the parent and spawns
*   NPROC worker tasks.  The first worker task is spawned on a specific machine.
*   The master program performs the matrix multiply by sending all of matrix B
*   to every worker task and then partitioning rows of matrix A among the
*   workers.  The worker tasks perform the actual multiplications and send back
*   to the master task their respective results.
*   NOTE1:  C and Fortran versions of this code differ because of the way
*     arrays are stored/passed.  C arrays are row-major order but Fortran
*     arrays are column-major order.
* PVM VERSION: 3.x
* AUTHOR: Roslyn Leibensperger
* LAST REVISED: 4/18/94 Blaise Barney
******************************************************************************/

#include <stdio.h>
#include "pvm3.h"		/* PVM version 3.0 include file */

#define NPROC 4			/* number of PVM worker tasks to spawn */
#define NRA 62 			/* number of rows in matrix A */
#define NCA 15			/* number of columns in matrix A */
#define NCB 7   		/* number of columns in matrix B */

main() int mtid,			/* PVM task id of master task */ 
    wtids[NPROC],        	/* array of PVM task ids for worker tasks */
    mtype,                     	/* PVM message type */
    rows,                      	/* rows of matrix A sent to each worker */
    averow, extra, offset,      /* used to determine rows sent to each worker */
    rcode, i, j;		/* misc */
double a[NRA][NCA], 		/* matrix A to be multiplied */
       b[NCA][NCB],      	/* matrix B to be multiplied */
       c[NRA][NCB];		/* result matrix C */
char thishost[35];		/* name of selected master */

/* enroll this task in PVM */
mtid = pvm_mytid();

/* The master task now spawns worker tasks by calling pvm_spawn.  The unique */
/* worker task ids are stored in the wtids array.  The first worker task is */
/* spawned on a specific machine.  The return code tells the number of tasks */
/* successfully spawned, and in this example, is not checked for errors.     */
for (i=0; i<NPROC; i++) {
  if (i==0) {
     printf ("Enter selected hostname - must match PVM config: ");
     scanf("%s", thishost);
     rcode = pvm_spawn("mm.worker", NULL, PvmTaskHost, thishost, 1, &wtids[0]);
     }
  else
     rcode = pvm_spawn("mm.worker", NULL, PvmTaskDefault, "", 1, &wtids[i]);
  }

/* initialize A and B */
for (i=0; i<NRA; i++)
   for (j=0; j<NCA; j++)
      a[i][j]= i+j;

for (i=0; i<NCA; i++)
   for (j=0; j<NCB; j++)
      b[i][j]= i*j;

averow = NRA/NPROC;
extra = NRA%NPROC;
offset = 0;
mtype = 1;

/* send data to the worker tasks */
for (i=0; i<NPROC; i++)	{			/* for each worker task */
  rows = (i < extra) ? averow+1 : averow;   	/* Find #rows to send from A */

  /* next call initializes send buffer and specifies to do XDR data format */
  /* conversion only in heterogenous environment */
  rcode = pvm_initsend(PvmDataDefault);    

  /* next four calls pack values into the send buffer */
  rcode = pvm_pkint(&offset, 1, 1);	  	/* starting pos. in matrix */
  rcode = pvm_pkint(&rows, 1, 1);	   	/* #rows of A to send */
  rcode = pvm_pkdouble(&a[offset][0], rows*NCA, 1); /* some rows from A */
  rcode = pvm_pkdouble(b, NCA*NCB, 1); 		/* all of B */

  /* send contents of send buffer to worker task  */
  rcode = pvm_send(wtids[i], mtype);

  offset = offset + rows;
}

/* wait for results from all worker tasks */
mtype = 2;					/* set message type */
for (i=0; i<NPROC; i++)	{			/* do once for each worker */
   rcode = pvm_recv(-1, mtype);			/* receive message from worker*/
   rcode = pvm_upkint(&offset, 1, 1);		/* starting pos. in matrix */
   rcode = pvm_upkint(&rows, 1, 1);		/* #rows sent */
   rcode = pvm_upkdouble(&c[offset][0], rows*NCB, 1); /* rows for matrix C */
   }

/* print results */
for (i=0; i<NRA; i++) { 
   printf("\n"); 
   for (j=0; j<NCB; j++) 
      printf("%6.2f   ", c[i][j]);
   }
printf ("\n");

/* task now exits from PVM */
rcode = pvm_exit();
}
