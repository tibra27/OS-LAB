#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
//#include <sys/types.h>
//#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>


#define	FIRST		1
#define	SECOND		2

int	shmID1,
	shmID2, 		// using 2 variables
	shrID; 		// identifier for shared memory to store shared value
key_t 	key1 = 54991, key2 = 78451 ,key3=678945;
int	*p1_inside,*p2_inside, *shared;


int main()
{
    shmID1 = shmget(key1, sizeof(int), IPC_CREAT | 0660); // flag
    if ( shmID1 < 0 ) {
        printf("Shared memory 1 could not be allocated. Quitting.\n");
        exit(1);
    }
    shmID2 = shmget(key2, sizeof(int), IPC_CREAT | 0660); // flag
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

    p1_inside = (int *)shmat(shmID1, NULL, 0);
    if (p1_inside == (int *)-1 )
    {
    	printf("Failed to attach p1_inside to first process. Quitting.\n");
        exit(-1);
    }
    p2_inside = (int *)shmat(shmID2, NULL, 0);
    if (p2_inside == (int *)-1 )
    {
    	printf("Failed to attach p2_inside to first process. Quitting.\n");
        exit(-1);
    }
    shared = (int *)shmat(shrID, NULL, 0);
    if (shared == (int *)-1 )
    {
    	printf("Failed to attach shared variable to first process. Quitting.\n");
        exit(-1);
    }
	*shared = 0;
	*p1_inside = 0;
	*p2_inside = 0;
    // Create First (child) process
    if (fork() == 0)                              //first child process execution
    {
    	p1_inside = (int *)shmat(shmID1, NULL, 0);
    	if (p1_inside == (int *)-1 )
		{
			printf("Failed to attach p1_inside to first process. Quitting.\n");
		    exit(-1);
		}
		p2_inside = (int *)shmat(shmID2, NULL, 0);
		if (p2_inside == (int *)-1 )
		{
			printf("Failed to attach p2_inside to first process. Quitting.\n");
		    exit(-1);
		}
    	shared = (int *)shmat(shrID, NULL, 0);
        if (shared == (int *)-1 )
        {
            printf("Failed to attach shared variable to first process. Quitting.\n");
            exit(-1);
        }
	// This is the code first process executes
		int i;
 		for(i=0; i < 20; i++)
 		{
			//*shared = *shared % 5;
        	while(*p2_inside)
			;	// Wait
			*p1_inside=1;
        	// Critical Section Begin
			*shared = *shared+1;

			printf("[%d] First: Critical Section (%2d).\n",i+1, *shared);
			*p1_inside=0;
		}
    }
    else                                        //parent process execution
    {
    	// Create second (child) process
    	if (fork() == 0)                        //second child process execution
    	{
    		p1_inside = (int *)shmat(shmID1, NULL, 0);
			if (p1_inside == (int *)-1 )
			{
				printf("Failed to attach p1_inside to second process. Quitting.\n");
				exit(-1);
			}
			p2_inside = (int *)shmat(shmID2, NULL, 0);
			if (p2_inside == (int *)-1 )
			{
				printf("Failed to attach p2_inside to second process. Quitting.\n");
				exit(-1);
			}
        	shared = (int *)shmat(shrID, NULL, 0);
        	if (shared == (int *)-1 )
        	{
            	printf("Failed to attach shared variable to second process. Quitting.\n");
            	exit(-1);
        	}
			// This is the code second process executes
			int i;
 			for(i=0; i < 20; i++)
 			{
        		while(*p1_inside)
				;	// Wait
				*p2_inside=1;
        		// Critical Section Begin
				*shared = *shared+1;
				printf("[%d] Second: Critical Section (%2d).\n",i+1, *shared);
				*p2_inside=0;
			}
    	}
    	else                                //parent process executes
    	{
    		int i;
 			for(i=0; i < 20; i++)
 			{
				*shared = *shared+12;
				printf("[%d] Parent: Critical Section (%2d).\n",i+1, *shared);
			}
    		wait(NULL);                 //2 waits for 2 child processes
    		wait(NULL);
   		}
    }
}
