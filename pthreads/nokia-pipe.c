#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
 
const char* fifopath = "./mkfifo.file";
unsigned int threadid;
 
// thread entry point
void* FifoWriterThread(void* arg)
{
   const char* buf = "somejunk";
	int fd;
   fd = open (fifopath, O_WRONLY);
   if(fd > 0 )
   {
      write(fd, buf, 9);     
      close(fd);
   }
   return 0;
}
 
void CreateThreadL()
{
   pthread_create(&threadid,(pthread_attr_t *)NULL,FifoWriterThread, NULL);
}
 
void OpenForRead()
{
   int err = mkfifo (fifopath, 0666);
   if(err != 0)
   {
       // probably file already exists, delete the file
       unlink(fifopath); 
       // try once more..
       err = mkfifo (fifopath, 0666);
       if(err != 0)
       {
            return;
       }
   }
   CreateThreadL();
   char buf[128];
	int fd;
   fd = open (fifopath, O_RDONLY);
   if ( fd > 0 )
       {
           return ;
       }
   err = read (fd, buf, 128);  
   close(fd);   
   unlink(fifopath);
}    
 
int main()
{
   OpenForRead();
   return 0;
}
