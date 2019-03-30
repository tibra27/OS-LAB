// gcc -pthread <name.c>
// This program creates two threads, each thread 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 

int	x = 0;		// Shared variable
int arr[5]={20,30,40,50,60};


#define	THREAD1		0
#define	THREAD2		1

#define	TRUE		1
#define FALSE		0
  
int choosing[7];
int ticket[7];  
  
void Initialization(void) { 
	//threadNumber = THREAD1;
	srand(time(NULL));
} 
int max_value()
{
	int mx=-100000;
	int i=0;
	for(i=0;i<10;i++)
	{
		if(ticket[i]>mx)
		mx=ticket[i];
	}
	return mx;
}
void	*func1(void *s) 
{ 
		int p=(int)s;
		int i=0;
		for(i=0;i<5;i++)
		{
			/* Take a ticket */
			choosing[p]=1;        /* begin ticket selection process */
			ticket[p]=max_value(ticket)+1;
			choosing[p]=0;       /* end ticket selection process */
			int j;
			/* Wait until turn of current ticket comes */
			for(j=0;j<7;j++)
			{
				if(j==p)
				{
					continue;
				}
				 /* Busy wait while Ti is choosing a ticket */
				while(choosing[j]!=0)   
					; //wait
				 /* Busy wait while current ticket value is lowest */
				while(ticket[j]!=0 && ticket[j]<ticket[p])
					; //wait
					/* In case of tie, favor smaller process number */
				if(ticket[j]==ticket[p] && j<p)
				{
					/* Busy wait until Ti exits critical section*/
					while(ticket[j]!=0)
						;//wait
				}
			}
			/*for(j=0;j<5;j++)
			{
				printf("arr[%d]=%d :: Thread 1 in Critical Section\n",j,arr[j]);
			}*/
			x = x+1;
			printf("[%2d] :  Thread in Critical Section (%d).\n",p,x);
			ticket[p]=0;
			// Random delay added
			int k;
			k = (int) ((3.0*rand())/RAND_MAX);
			sleep(k);
	
		}

} 
  
  

int main() { 
    	pthread_t arr[7]; 
    	// Initialized the lock then fork 2 threads 
		//Initialization();
  
    	// Create two threads (both run func)
    	int i=0;
    	for(i=0;i<7;i++)
    	{
    		pthread_create(&arr[i], NULL, func1, (void *)i); 
    	}  
    	
  
	// Wait for the threads to end. 
		for(i=0;i<7;i++)
    	{
    			pthread_join(arr[i], NULL); 
    	} 
    
  
  
    	return 0; 
} 

