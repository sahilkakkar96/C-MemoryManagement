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
	char buf[100];
	char *token,ar[100][100];
 	int q;
 	
	int fifo_main,fifo_mem;
	for(i=0;i<1023;i++)
	*((int*)emulated_memory + i) = 0;  
	
	
	  
	while(1)
{		  
	fifo_mem=open("fifo_mem",O_RDWR);
	if(fifo_mem < 0) {
	  printf("Error in opening file");
	  exit(-1);
	  }
	read(fifo_mem, buf, 100);
	close(fifo_mem);
	sleep(3);
	q=0;
	size_t rem= strlen(buf);
	if(buf[--rem]=='\n')
	buf[rem]='\0';
	
	token = strtok(buf," ");
	while(token!=NULL)
	{
		strcpy(*(ar+q),token);
		q++;
		token = strtok(NULL," ");
	}
    if(strcmp(*ar,"allocate")==0)
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
		fifo_main=open("fifo_main",O_RDWR);
		if(fifo_main < 0) {
		  printf("Error in opening file");
		  exit(-1);
		  }
    	write(fifo_main,&phyadd,sizeof(int));
    	close(fifo_main);
    }
    else if(strcmp(*ar,"kill")!=0)
    {
    	
    }
    else if(strcmp(*ar,"read")!=0)
    {
    	int index;
    	char *c;
    	index = strtol(*(ar+1),&c,10);
    	if(mem_used[index]==1)
    	{
    		int item;
    		item = *((int*)emulated_memory + index);
    		if(fifo_main < 0) {
			  printf("Error in opening file");
			  exit(-1);
			  }
	    	write(fifo_main,&item,sizeof(int));
    	}
    }
    else if(strcmp(*ar,"write")!=0)    //write 12413532623153   000000000000000000000000001000000000000000  5
    {
    	
    }
    else if(strcmp(*ar,"exit")==0)
    {
    	close(fifo_main);
	 	close(fifo_mem);
    	exit(EXIT_SUCCESS);
    }
    else {}
}
	
	return -1;
}