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

static int np=0,pf1=0,pf2=0,pf3=0,pf4=0;
unsigned long tidArray[4];
pthread_t tid[4];

void *processCreate1(void *vargp)
{	
    while(pf1==1)
    {
    }
    return NULL; 
}

void *processCreate2(void *vargp)
{	
    while(pf2==1)
    {
    }
    return NULL; 
}
void *processCreate3(void *vargp)
{	
    while(pf3==1)
    {
    }
    return NULL; 
}
void *processCreate4(void *vargp)
{	
    while(pf4==1)
    {
    }
    return NULL; 
}

int main(int argc, char** argv) {
   
   	char app[30],*b;
 
    while(1)
{
    printf("Which application to run - ");
    fgets(app , sizeof(app), stdin);

    if(strcmp(app,"create\n")==0)
    {
    	if(pf1==0)
    			{
    				np=0;
    				pf1=1;
    			}
    	else if(pf2==0)
    			{
    				np=1;
    				pf2=1;
    			}
    	else if(pf3==0)
    			{
    				np=2;
    				pf3=1;
    			}
    	else if(pf4==0)
    			{
    				np=3;
    				pf4=1;
    			} 
    	else np=5;
    	if(np==0)
    	{
    		pthread_create(&tid[np], NULL, processCreate1, NULL);
	     	tidArray[np]=(unsigned long)tid[np];
    	}
    	else if(np==1)
    	{
    		pthread_create(&tid[np], NULL, processCreate2, NULL);
	     	tidArray[np]=(unsigned long)tid[np];
    	}
    	else if(np==2)
    	{
    		pthread_create(&tid[np], NULL, processCreate3, NULL);
	     	tidArray[np]=(unsigned long)tid[np];
    	}
    	else if(np==3)
    	{
    		pthread_create(&tid[np], NULL, processCreate4, NULL);
	     	tidArray[np]=(unsigned long)tid[np];
    	}
    	else 
    	{
    		printf("Can't create more than four processes, kill some process before creating more\n"); 
    	}
    }
    else if(strstr(app,"kill")!=0)
    {
    	char *token,arr[100][100];
    	int i,q=0;
    	long proc_id;
    	
    	size_t rem= strlen(app);
		if(app[--rem]=='\n')
	    app[rem]='\0';
	    
		token = strtok(app," ");
		while(token!=NULL)
		{
			strcpy(*(arr+q),token);
			q++;
			token = strtok(NULL," ");
		}
		proc_id= atol(*(arr+1));
    	
    	for(i=0;i<4;i++)
    	{
    		if(tidArray[i]==proc_id)
    		{
    			if(i==0)
    			{
    				pf1=0;
    				tidArray[0]=0;
    			}
    			else if(i==1)
    			{
    				pf2=0;
    				tidArray[1]=0;
    			}
    			else if(i==2)
    			{
    				pf3=0;
    				tidArray[2]=0;
    			}
    			else
    			{
    				pf4=0;
    				tidArray[3]=0;
    			} 
    			pthread_join(tid[i], (void**)&b);
    		}
    		//pthread_cancel(tid[i]);
    	}
    	
    }
    else if(strcmp(app,"list\n")==0)
    {
    	int i;
    	for(i=0;i<4;i++)
    	{
    		printf("%lu \n",tidArray[i]);
    	}
    } 
    else if(strcmp(app,"mem\n")==0)
    {
    	
    }
    else if(strcmp(app,"allocate\n")==0)
    {
    	
    }
    else if(strcmp(app,"read\n")==0)
    {
    	
    }
    else if(strcmp(app,"write\n")==0)
    {
    	
    }
    else if(strcmp(app,"exit\n")==0)
    {
    	exit(EXIT_SUCCESS);
    }
    else printf("Enter Correct Command to Run\n");   
}
    
	return -1;
}