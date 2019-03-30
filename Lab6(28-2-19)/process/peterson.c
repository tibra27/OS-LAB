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
	shmID2,
	shmID3,      // using 3 variables
	shrID; 		// identifier for shared memory to store shared value
key_t 	key1 = 54991, key2 = 78451 ,key3=678945, key4=345678;
int	*favored_process,*p1_wantstoenter,*p2_wantstoenter, *shared;


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
    shmID3 = shmget(key4, sizeof(int), IPC_CREAT | 0660); // flag
    if ( shmID2 < 0 ) {
        printf("Shared memory 4 could not be allocated. Quitting.\n");
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
    favored_process = (int *)shmat(shmID3, NULL, 0);
    if (p1_wantstoenter == (int *)-1 )
    {
    	printf("Failed to attach favored_process to first process. Quitting.\n");
        exit(-1);
    }
    p1_wantstoenter = (int *)shmat(shmID1, NULL, 0);
    if (p1_wantstoenter == (int *)-1 )
    {
    	printf("Failed to attach p1_wantstoenter to first process. Quitting.\n");
        exit(-1);
    }
    p2_wantstoenter = (int *)shmat(shmID2, NULL, 0);
    if (p2_wantstoenter == (int *)-1 )
    {
    	printf("Failed to attach p2_wantstoenter to first process. Quitting.\n");
        exit(-1);
    }
    shared = (int *)shmat(shrID, NULL, 0);
    if (shared == (int *)-1 )
    {
    	printf("Failed to attach shared variable to first process. Quitting.\n");
        exit(-1);
    }
	*shared = 0;
	*favored_process=1;
	*p1_wantstoenter = 0;
	*p2_wantstoenter = 0;
    // Create First (child) process
    if (fork() == 0)                              //first child process execution
    {
         favored_process = (int *)shmat(shmID3, NULL, 0);
        if (p1_wantstoenter == (int *)-1 )
        {
        	printf("Failed to attach favored_process to first process. Quitting.\n");
            exit(-1);
        }
    	p1_wantstoenter = (int *)shmat(shmID1, NULL, 0);
    	if (p1_wantstoenter == (int *)-1 )
		{
			printf("Failed to attach p1_wantstoenter to first process. Quitting.\n");
		    exit(-1);
		}
		p2_wantstoenter = (int *)shmat(shmID2, NULL, 0);
		if (p2_wantstoenter == (int *)-1 )
		{
			printf("Failed to attach p2_wantstoenter to first process. Quitting.\n");
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
 			*p1_wantstoenter=1;
 			*favored_process=2;
		//	*shared = *shared % 5;
        	while(*p2_wantstoenter && *favored_process==2)
        	    ;  //wait

        	// Critical Section Begin
			*shared = *shared+1;

			printf("[%d] First: Critical Section (%2d).\n",i+1, *shared);

			*p1_wantstoenter=0;
		}
    }
    else                                        //parent process execution
    {
    	// Create second (child) process
    	if (fork() == 0)                        //second child process execution
    	{
    	    favored_process = (int *)shmat(shmID3, NULL, 0);
            if (p1_wantstoenter == (int *)-1 )
            {
            	printf("Failed to attach favored_process to second process. Quitting.\n");
                exit(-1);
            }
    		p1_wantstoenter = (int *)shmat(shmID1, NULL, 0);
			if (p1_wantstoenter == (int *)-1 )
			{
				printf("Failed to attach p1_wantstoenter to second process. Quitting.\n");
				exit(-1);
			}
			p2_wantstoenter = (int *)shmat(shmID2, NULL, 0);
			if (p2_wantstoenter == (int *)-1 )
			{
				printf("Failed to attach p2_wantstoenter to second process. Quitting.\n");
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
 				*p2_wantstoenter=1;
 				*favored_process=1;
        		while(*p1_wantstoenter && *favored_process==1)
        	        ;   //wait

        		// Critical Section Begin
				*shared = *shared+1;
				printf("[%d] Second: Critical Section (%2d).\n",i+1, *shared);

				*p2_wantstoenter=0;
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
