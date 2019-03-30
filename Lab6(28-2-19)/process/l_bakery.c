#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
//#include <sys/types.h>
//#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define n 6
#define	FIRST		1
#define	SECOND		2

int	shmID1,
	shmID2,
	shrID; 		// identifier for shared memory to store shared value
key_t 	key1 = 54991, key2 = 78451 ,key3=678945;
int	*ticket,*choosing,*shared;



int max_value()
{
    int mx =-100000;
    int i;
    for(i=0;i<n;i++)
    {
        int val = ticket[i];
        if(val > mx)
        {
            mx = val;
        }
    }
    return mx;
}

int main()
{
    srand(time(NULL));
    int main_pid = getpid();

    shmID1 = shmget(key1, n*sizeof(int), IPC_CREAT | 0660); // flag
    if ( shmID1 < 0 ) {
        printf("Shared memory 1 could not be allocated. Quitting.\n");
        exit(1);
    }
    shmID2 = shmget(key2, n*sizeof(int), IPC_CREAT | 0660); // flag
    if ( shmID2 < 0 ) {
        printf("Shared memory 2 could not be allocated. Quitting.\n");
        exit(1);
    }
    shrID = shmget(key3, sizeof(int), IPC_CREAT | 0660); // flag
    if ( shrID < 0 ) {
        printf("Shared memory 3 could not be allocated. Quitting.\n");
        exit(1);
    }

    // Initialization is done in parent process
    	// shmat : attach shared memory segment. Returns (void *)-1 on failure else
    	// Attaches shared segment identified with shmID to an address
    	// processNumber in process address space
    ticket = (int *)shmat(shmID1, NULL, 0);
    if (ticket == (int *)-1 )
    {
    	printf("Failed to attach ticket to main process. Quitting.\n");
        exit(-1);
    }
    choosing = (int *)shmat(shmID2, NULL, 0);
    if (choosing == (int *)-1 )
    {
    	printf("Failed to attach choosing to main process. Quitting.\n");
        exit(-1);
    }
    shared = (int *)shmat(shrID, NULL, 0);
    if (shared == (int *)-1 )
    {
    	printf("Failed to attach shared variable to main process. Quitting.\n");
        exit(-1);
    }
	*shared = 0;
	int j=0;
	for(j=0;j<n;j++)
	{
	    ticket[j]=0;
	    choosing[j]=0;
	}
	fork();
	fork();
	fork();

	ticket = (int *)shmat(shmID1, NULL, 0);
    if (ticket == (int *)-1 )
    {
    	printf("Failed to attach ticket to child process. Quitting.\n");
        exit(-1);
    }
    choosing = (int *)shmat(shmID2, NULL, 0);
    if (choosing == (int *)-1 )
    {
    	printf("Failed to attach choosing to child process. Quitting.\n");
        exit(-1);
    }
    shared = (int *)shmat(shrID, NULL, 0);
    if (shared == (int *)-1 )
    {
    	printf("Failed to attach shared variable to child process. Quitting.\n");
        exit(-1);
    }

    int x=getpid()-main_pid;
    for(j=0;j<7;j++)
    {
        choosing[x]=1;
        ticket[x]=max_value(ticket)+1;
        choosing[x]=0;
        int i;
        for(i=0;i<n;i++)
        {
            if(i==x)
            {
                continue;
            }
            while(choosing[i]!=0)
            ;
            while(ticket[i]!=0 && ticket[i]<ticket[x])
            ;
            if(ticket[i]==ticket[x] && i<x)
            {
                while(ticket[i]!=0)
                ;
            }
        }
        *shared = *shared+1;
		printf("[%d] %d: Critical Section (%2d).\n",i+1,x, *shared);
        ticket[x]=0;
    }
    wait(NULL);
}
