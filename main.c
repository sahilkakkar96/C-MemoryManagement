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

int cse320_virt_to_phys(int v)
{
	v=v>>12;
	return v;
}


unsigned int cse320_malloc(int id)
{
	unsigned int va; int i,j;
	int fifo_main,fifo_mem;
	int phyadd;
	
	fifo_mem = open("fifo_mem",O_RDWR);
	
	if(fifo_mem<1) {
	 printf("Error opening file");
	 }
	write(fifo_mem,"allocate",sizeof("allocate")); 
	close(fifo_mem);

	fifo_main = open("fifo_main",O_RDWR);
	if(fifo_main<1) {
	 printf("Error opening file");
	 }
	read(fifo_main,&phyadd,sizeof(int));
	close(fifo_main);
	
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
   
   	char app[100],*b;
 	char *token,arr[100][100];
 	int q;

   while(1)
{
l1: printf("Which command to run - ");
    fgets(app , sizeof(app), stdin);
    
    if(strcmp(app,"\n")==0)
    {
    	printf("Enter Correct Command\n");
    	goto l1;
    }
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
    	if(q!=1)
    	{
    		printf("Enter Correct Command Arguments to Run\n");
    		goto l1;
    	} 
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
    	if(q!=2)
    	{
    		printf("Enter Correct Command Arguments to Run\n");
    		goto l1;
    	} 
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
    		for(i=0;i<4;i++)
	    	{
	    		if(tidArray[i]==proc_id)
	    		{
	    			if(i==0)
	    			{
	    				pf1=0;
	    				tidArray[0]=0;
	    				flpt[0].p_id = 0;
	    				int cl=0;
	    				while(flpt[0].sl->val[cl]==1)
	    				{
	    					int fifo_mem;
			
							fifo_mem = open("fifo_mem",O_RDWR);
							
							if(fifo_mem<1) {
							 printf("Error opening file");
							 }
							 char msg[100],strnum[30];
							 sprintf(strnum, "%d", flpt[0].sl->phy_add[cl]);
							 strcpy(msg,"kill ");
							 strcat(msg, strnum);
							 size_t rem= strlen(msg);
							msg[rem]='\0';
							 write(fifo_mem,msg,strlen(msg)+1); 
							
	    					vad1[cl]=0;
	    					va1=0;
	    					flpt[0].sl->val[cl] = 0;
	    					cl++;
	    				}
	    					    				
	    			}
	    			else if(i==1)
	    			{
	    				pf2=0;
	    				tidArray[1]=0;
	    				flpt[1].p_id = 0;
	    				int cl=0;
	    				while(flpt[1].sl->val[cl]==1)
	    				{
	    					vad2[cl]=0;
	    					va2=0;
	    					flpt[1].sl->val[cl] = 0;
	    					cl++;
	    				}
	    			}
	    			else if(i==2)
	    			{
	    				pf3=0;
	    				tidArray[2]=0;
	    				flpt[2].p_id = 0;
	    				int cl=0;
	    				while(flpt[2].sl->val[cl]==1)
	    				{
	    					vad3[cl]=0;
	    					va3=0;
	    					flpt[2].sl->val[cl] = 0;
	    					cl++;
	    				}
	    			}
	    			else
	    			{
	    				pf4=0;
	    				tidArray[3]=0;
	    				flpt[3].p_id = 0;
	    				int cl=0;
	    				while(flpt[3].sl->val[cl]==1)
	    				{
	    					vad4[cl]=0;
	    					va4=0;
	    					flpt[3].sl->val[cl] = 0;
	    					cl++;
	    				}
	    			} 
	    			pthread_join(tid[i], (void**)&b);
	    		}
	    	}
    	}
    	
    }
    else if(strcmp(*arr,"list")==0)
    {
    	if(q!=1)
    	{
    		printf("Enter Correct Command Arguments to Run\n");
    		goto l1;
    	} 
    	int i;
    	for(i=0;i<4;i++)
    	{
    		printf("%lu\n",tidArray[i]);
    	}
    } 
    else if(strcmp(*arr,"mem")==0)
    {
    	if(q!=2)
    	{
    		printf("Enter Correct Command Arguments to Run\n");
    		goto l1;
    	} 
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
    	if(q!=2)
    	{
    		printf("Enter Correct Command Arguments to Run\n");
    		goto l1;
    	} 
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
    	if(q!=3)
    	{
    		printf("Enter Correct Command Arguments to Run\n");
    		goto l1;
    	} 
    	long proc_id; 
    	char *c;
    	proc_id = atol(*(arr+1));
    	int reqva;
    	reqva = strtol(*(arr+2),&c,2);
    	int cpa = cse320_virt_to_phys(reqva);			//got process ID for lvl 1 table and index for lvl 2 table
    	
    	int i;
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
    		char strnum[30];
    		if(flpt[i].sl->val[cpa]!=1)
    		{
    			printf("No Physical Address at that Index\n");
    			goto l1;
    		}
    		int a = flpt[i].sl->phy_add[cpa];			//got actual physical address
    		sprintf(strnum, "%d", a);
    		
    		int fifo_main,fifo_mem;
			
			fifo_mem = open("fifo_mem",O_RDWR);
			
			if(fifo_mem<1) {
			 printf("Error opening file");
			 }
			 char msg[100];
			 strcpy(msg,"read ");
			 strcat(msg, strnum);
			 size_t rem= strlen(msg);
			msg[rem]='\0';
			 write(fifo_mem,msg,strlen(msg)+1); 
			int item;
			 fifo_main = open("fifo_main",O_RDWR);
			if(fifo_main<1) {
			 printf("Error opening file");
			 }
			 read(fifo_main,&item,sizeof(int));

			 printf("Data at given address is : %d\n",item);
			 close(fifo_mem);
			 close(fifo_main);
    	}
    }
    else if(strcmp(*arr,"write")==0)
    {
    	if(q!=4)
    	{
    		printf("Enter Correct Command Arguments to Run\n");
    		goto l1;
    	} 
    	long proc_id; 
    	char *c;
    	proc_id = atol(*(arr+1));
    	int reqva;
    	reqva = strtol(*(arr+2),&c,2);
    	int cpa = cse320_virt_to_phys(reqva);			//got process ID for lvl 1 table and index for lvl 2 table
    	
    	int i;
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
    		char strnum[30];
    		if(flpt[i].sl->val[cpa]!=1)
    		{
    			printf("No Physical Address at that Index\n");
    			goto l1;
    		}
    		int a = flpt[i].sl->phy_add[cpa];			//got actual physical address
    		sprintf(strnum, "%d", a);
    		
    		int fifo_mem;
			
			fifo_mem = open("fifo_mem",O_RDWR);
			
			if(fifo_mem<1) {
			 printf("Error opening file");
			 }
			 char msg[100];
			 strcpy(msg,"write ");
			 strcat(msg, strnum);
			 strcat(msg, " ");
			 strcat(msg, *(arr+3));
			 size_t rem= strlen(msg);
     		 msg[rem]='\0';
			 write(fifo_mem,msg,strlen(msg)+1); 
			sleep(5);
			close(fifo_mem);
    	}
    }
    else if(strcmp(*arr,"exit")==0)
    {
    	exit(EXIT_SUCCESS);
    }
	else printf("Enter Correct Command to Run\n");	
}
    
	return -1;
}