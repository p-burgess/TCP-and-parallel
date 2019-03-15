// In this  program a scatter  operation  distributes  the
//  individual  elements  of an array  of  integers  among  the
//  processes.   Each  process  modifies  the  value  it  receives
// and  then  participates  in a gather  operation  that
//  collects  the  modified  data in the  master  process  where
// they  are  once  again  assembled  into  the  original  array.
#include  <stdio.h>
#include  <mpi.h>
const  int  MASTER_RANK = 0;
// Rank of of the  master  process
// Set  the  number  of  elements  that  should  be
// sent to each  process.   The  number  of  elements
// in the  entire  array  will be a multiple  of
// this  value.
const  int  SIZE_PER_PROCESS = 2;
int  main( int argc , char* argv[] )
{
//  Initialize  the  MPI  system  and  determine  the
//  number  of  collaborating  processes  and  the  rank
// of the  current  process.
int  numProc , rank;
MPI_Init( &argc , &argv );
MPI_Comm_size( MPI_COMM_WORLD , &numProc  );
MPI_Comm_rank( MPI_COMM_WORLD , &rank );
// The  master  process  allocates  and  initializes
// the u array  of  integers.   Other  processes  will
// need a valid  pointer  in their  scatter/gather
// calls , but it will be  ignored  and so can be
// NULL.   Each  process  needs  array v to  receive
// data  into.
int* u = NULL;
int* v = new int [SIZE_PER_PROCESS ];
if ( rank ==  MASTER_RANK )
{
//  Master  process  allocates  array  and  fills
// it with  data.   The  values  in the  array  are
// 100 * (rank +1)  plus  the an  offset  from
// 0.. SIZE_PER_PROCESS.
u = new int [numProc * SIZE_PER_PROCESS ];
printf( "Master: Scattering =" );
for ( int i = 0; i < numProc; i++ )
{
for ( int j = 0; j < SIZE_PER_PROCESS; j++ )
{
int k = i * SIZE_PER_PROCESS + j;
u[k] = 100 * ( i + 1 ) + j;
printf( "%5d", u[k] );
}
}
printf( "\n" );
}
// Each  process  participates  in the  scatter;
// the  first  three  parameters  ("the  source ")
// are  used if the  process ' rank  matches  the
// next -to-last  parameter.   All  processes  use
// the  next  three  parameters  ("the  destination ").
MPI_Scatter( u, SIZE_PER_PROCESS , MPI_INT ,v, SIZE_PER_PROCESS , MPI_INT ,MASTER_RANK , MPI_COMM_WORLD  );
// Each  process , including  the  master , adds a
//  distinguishable  value  to  received  data.
printf( "Process  %2d: ", rank );
for ( int i = 0; i < SIZE_PER_PROCESS; i++ )
{
printf( " (%4d", v[i] );
v[i] += 1000 * ( rank + 1 );
printf( " -> %4d)", v[i] );
}
printf( "\n" );
// Each  process  participates  in the  gather.   Source
//  parameters  are  used by each  process  but  only  the
//  master  process  makes  use of  destination  parameters.
MPI_Gather( v, SIZE_PER_PROCESS , MPI_INT , u, SIZE_PER_PROCESS , MPI_INT ,MASTER_RANK , MPI_COMM_WORLD  );
if ( rank ==  MASTER_RANK ) {
//  Master  process  displays  assembled  data
printf( "Master: Received    =" );
for ( int i = 0; i < numProc; i++ ) {
for ( int j = 0; j < SIZE_PER_PROCESS; j++ )
{
int k = i * SIZE_PER_PROCESS + j;
printf( "%5d", u[k] );
}
}
printf( "\n" );
}
// clean  up
delete  [] u;
delete  [] v;
MPI_Finalize ();
return  0;
}
