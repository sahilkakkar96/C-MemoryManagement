#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
int main(int argc, char** argv)
{
int file1,file2;
file1 = mkfifo("fifo_main",0666); 
if(file1<0) {
 printf("Unable to create a fifo\n");
 exit(-1);
 }

file2 = mkfifo("fifo_mem",0666);

if(file2<0) {
 printf("Unable to create a fifo\n");
 exit(-1);
 }
 return 0;
}
