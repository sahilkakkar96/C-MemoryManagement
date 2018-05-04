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

int main(int argc, char** argv) {
   
   	sleep(5);
	void* emulated_memory = malloc(1024);
	int i,phyadd,mem_used[1024]; 
	char buf[50];
	int fifo_main,fifo_mem;
	for(i=0;i<1023;i++)
	*((int*)emulated_memory + i) = 0;  
	
	fifo_main=open("fifo_main",O_RDWR);
	if(fifo_main < 0) {
	  printf("Error in opening file");
	  exit(-1);
	  }
	  
	fifo_mem=open("fifo_mem",O_RDWR);
	if(fifo_mem < 0) {
	  printf("Error in opening file");
	  exit(-1);
	  }
	  
	while(1)
{
	read(fifo_mem, buf, 50);

    if(strcmp(buf,"allocate")==0)
    {
    	i=0;
    	while(i<1023)
		{
			if(mem_used[i]==1) 
			i++;
			else if(i%4==0)
			{
				phyadd= i;
				mem_used[i]=1;
				break;
			}
			else i++;
			
		}
		
    	write(fifo_main,&phyadd,sizeof(int));
    }
    else if(strcmp(buf,"kill")!=0)
    {
    	
    }
    else if(strcmp(buf,"exit")==0)
    {
    	close(fifo_main);
	 	close(fifo_mem);
    	exit(EXIT_SUCCESS);
    }
    else {}
}
	
	return -1;
}