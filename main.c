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


struct second_level_pt {
	int phy_add[1024];
	int val[1024];
};
struct second_level_pt slpt1,slpt2,slpt3,slpt4;

struct first_level_pt {
    struct second_level_pt *sl;
    int valid;
    long p_id;
}flpt[4];

static int np=0,pf1=0,pf2=0,pf3=0,pf4=0,va1=0,va2=0,va3=0,va4=0;
unsigned long tidArray[4];
int vad1[256];
int vad2[256];
int vad3[256];
int vad4[256];
pthread_t tid[4];

int ptov(int n) {
    int v=0;
    n=n<<12;
    v = v|n;
    return v;  
}

unsigned int cse320_malloc(int id)
{
	unsigned int va; int i,j;
	int fifo_main,fifo_mem;
	int phyadd;
	fifo_main = open("fifo_main",O_RDWR);
	if(fifo_main<1) {
	 printf("Error opening file");
	 }
	fifo_mem = open("fifo_mem",O_RDWR);
	
	if(fifo_mem<1) {
	 printf("Error opening file");
	 }
	write(fifo_mem,"allocate",sizeof("allocate")); 

	read(fifo_main,&phyadd,sizeof(int));
	
	j=id;
	for(i=0;i<1024;i++)
	{
		if( flpt[j].sl->val[i] == 0)
		{
			int k;
			flpt[j].sl->phy_add[i] = phyadd;
			flpt[j].sl->val[i] = 1;
			va = ptov(i);
			if(j==0)
			{
				k=1;
				if(va1==0)
				{
					vad1[va1]=va;
					va1++;
				}
				else 
				{
					while(k<256 && vad1[k]!=0)
					{k++;}
					if(k<256)
					{
						vad1[k]=va;
						va1++;
					}
				}
				
			}
			else if(j==1)
			{
				k=1;
				if(va2==0)
				{
					vad2[va2]=va;
					va2++;
				}
				else
				{
					while(k<256 && vad2[k]!=0)
					{k++;}
					if(k<256)
					{
						vad2[k]=va;
						va2++;
					}
				}
				
			}
			else if(j==2)
			{
				k=1;
				if(va3==0)
				{
					vad3[va3]=va;
					va3++;
				}
				else
				{
					while(k<256 && vad3[k]!=0)
					{k++;}
					if(k<256)
					{
						vad3[k]=va;
						va3++;
					}
				}
				
			}
			else if(j==3)
			{
				k=1;
				if(va4==0)
				{
					vad4[va4]=va;
					va4++;
				}
				else
				{
					while(k<256 && vad4[k]!=0)
					{k++;}
					if(k<256)
					{
						vad4[k]=va;
						va4++;
					}
				}
				
			}
			break; 
		}
	}
	
	close(fifo_main);
	close(fifo_mem);
	return va;
}


void *processCreate1(void *vargp)
{	
	flpt[0].sl = &slpt1;
	flpt[0].valid = 1;
	
    while(pf1==1)
    {
    }
    return NULL; 
}

void *processCreate2(void *vargp)
{	
	flpt[1].sl = &slpt2;
	flpt[1].valid = 1;
    while(pf2==1)
    {
    }
    return NULL; 
}
void *processCreate3(void *vargp)
{	
	flpt[2].sl = &slpt3;
	flpt[2].valid = 1;
    while(pf3==1)
    {
    }
    return NULL; 
}
void *processCreate4(void *vargp)
{	
	flpt[3].sl = &slpt4;
    flpt[3].valid = 1;
    while(pf4==1)
    {
    }
    return NULL; 
}

int main(int argc, char** argv) {
   
   	char app[30],*b;
 	char *token,arr[100][100];
 	int q;
l1:   printf("Enter Correct Command Arguments to Run\n");
   while(1)
{
    printf("Which command to run - ");
    fgets(app , sizeof(app), stdin);
    
    q=0;
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
	
    if(strcmp(*arr,"create")==0)
    {
    	if(q>1) goto l1;
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
	     	flpt[0].p_id = (unsigned long)tid[np];
    	}
    	else if(np==1)
    	{
    		pthread_create(&tid[np], NULL, processCreate2, NULL);
	     	tidArray[np]=(unsigned long)tid[np];
	     	flpt[1].p_id = (unsigned long)tid[np];
    	}
    	else if(np==2)
    	{
    		pthread_create(&tid[np], NULL, processCreate3, NULL);
	     	tidArray[np]=(unsigned long)tid[np];
	     	flpt[2].p_id = (unsigned long)tid[np];
    	}
    	else if(np==3)
    	{
    		pthread_create(&tid[np], NULL, processCreate4, NULL);
	     	tidArray[np]=(unsigned long)tid[np];
	     	flpt[3].p_id = (unsigned long)tid[np];
    	}
    	else 
    	{
    		printf("Can't create more than four processes, kill some process before creating more\n"); 
    	}
    }
    else if(strcmp(*arr,"kill")==0)
    {
    	if(q>2) goto l1;
    	int i;
    	long proc_id;
    	proc_id= atol(*(arr+1));
    	
    	for(i=0;i<4;i++)
    	{
    		if(tidArray[i]==proc_id)
    		{
    			if(i==0)
    			{
    				pf1=0;
    				tidArray[0]=0;
    				flpt[0].p_id = 0;
    			}
    			else if(i==1)
    			{
    				pf2=0;
    				tidArray[1]=0;
    				flpt[1].p_id = 0;
    			}
    			else if(i==2)
    			{
    				pf3=0;
    				tidArray[2]=0;
    				flpt[2].p_id = 0;
    			}
    			else
    			{
    				pf4=0;
    				tidArray[3]=0;
    				flpt[3].p_id = 0;
    			} 
    			pthread_join(tid[i], (void**)&b);
    		}
    	}
 //TODO : Check for wrong ID
    	
    }
    else if(strcmp(*arr,"list")==0)
    {
    	if(q>1) goto l1;
    	int i;
    	for(i=0;i<4;i++)
    	{
    		printf("%lu\n",tidArray[i]);
    	}
    } 
    else if(strcmp(*arr,"mem")==0)
    {
    	if(q>2) goto l1;
    	long proc_id;
    	proc_id= atol(*(arr+1));
    	int i,j;
    	i=0;
    	while(i<4)
    	{
    		if(proc_id!=flpt[i].p_id)
    		{
    			i++;
    		}
    		else break;
    	}
    	if(i==4)
    	{
    		printf("Wrong Process ID\n");
    	}
    	else 
    	{
    		if(i==0)
    		{
    			for(i=0;i<va1;i++)
		    	{
		    		for(j=31; j>=0; j--) 
		    		printf("%d",((vad1[i] >> j) & 1));
		    		printf("\n");
		    	}
    		}
    		else if(i==1)
    		{
    			for(i=0;i<va2;i++)
		    	{
		    		for(j=31; j>=0; j--) 
		    		printf("%d",((vad2[i] >> j) & 1));
		    		printf("\n");
		    	}
    		}
    		else if(i==2)
    		{
    			for(i=0;i<va3;i++)
		    	{
		    		for(j=31; j>=0; j--) 
		    		printf("%d",((vad3[i] >> j) & 1));
		    		printf("\n");
		    	}
    		}
    		else if(i==3)
    		{
    			for(i=0;i<va4;i++)
		    	{
		    		for(j=31; j>=0; j--) 
		    		printf("%d",((vad4[i] >> j) & 1));
		    		printf("\n");
		    	}
    		}
    		
    	}
    	
    }
    else if(strcmp(*arr,"allocate")==0)
    {
    	if(q>2) goto l1;
    	int i;
    	long proc_id;
    	proc_id= atol(*(arr+1));
    	i=0;
    	while(i<4)
    	{
    		if(proc_id!=flpt[i].p_id)
    		{
    			i++;
    		}
    		else break;
    	}
    	if(i==4)
    	{
    		printf("Wrong Process ID\n");
    	}
    	else 
    	{
    		cse320_malloc(i);
    	}
    }
    else if(strcmp(*arr,"read")==0)
    {
    	if(q>3) goto l1;
    }
    else if(strcmp(*arr,"write")==0)
    {
    	if(q>4) goto l1;
    }
    else if(strcmp(*arr,"exit")==0)
    {
    	exit(EXIT_SUCCESS);
    }
	else printf("Enter Correct Command to Run\n");	
}
    
	return -1;
}