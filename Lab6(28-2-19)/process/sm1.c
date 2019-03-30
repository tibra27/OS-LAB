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

int	shmID, 		// identifier for shared memory to store processNumber
	shrID; 		// identifier for shared memory to store shared value
key_t 	key1 = 54991, key2 = 784519;
int	*processNumber, *shared;


int main()
{
    shmID = shmget(key1, sizeof(int), IPC_CREAT | 0660); // flag
    if ( shmID < 0 )
    {
        printf("Shared memory 1 could not be allocated. Quitting.\n");
        exit(1);
    }

    shrID = shmget(key2, sizeof(int), IPC_CREAT | 0660); // flag
    if ( shrID < 0 )
    {
        printf("Shared memory 2 could not be allocated. Quitting.\n");
        exit(1);
    }

    // Initialization is done in parent process
    	// shmat : attach shared memory segment. Returns (void *)-1 on failure else
    	// Attaches shared segment identified with shmID to an address
    	// processNumber in process address space
    processNumber = (int *)shmat(shmID, NULL, 0);
    if (processNumber == (int *)-1 )
    {
        printf("Failed to attach processNumber to first process. Quitting.\n");
        exit(-1);
    }
	shared = (int *)shmat(shrID, NULL, 0);
    if (shared == (int *)-1 )
    {
            printf("Failed to attach shared variable to first process. Quitting.\n");
            exit(-1);
    }
	*shared = 0;
	*processNumber = FIRST;


    // Create First (child) process
    if (fork() == 0)
    {
    	processNumber = (int *)shmat(shmID, NULL, 0);
        if (processNumber == (int *)-1 )
        {
            printf("Failed to attach processNumber to first process. Quitting.\n");
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
		    	while(*processNumber == SECOND)
				;	// Wait

		    	// Critical Section Begin
			*shared = *shared+1;

			printf("[%d] First: Critical Section (%2d).\n",i+1, *shared);
			*processNumber = SECOND;
		}
    }
    else
    {
    // Create second (child) process
		if (fork() == 0)
		{
			processNumber = (int *)shmat(shmID, NULL, 0);
		    if (processNumber == (int *)-1 )
		    {
		        printf("Failed to attach processNumber to second process. Quitting.\n");
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
					while(*processNumber == FIRST)
					;	// Wait

					// Critical Section Begin
				*shared = *shared+1;
				printf("[%d] Second: Critical Section (%2d).\n",i+1, *shared);
				*processNumber = FIRST;
			}
    	}
		else
		{
				int i;
			 	for(i=0; i < 20; i++)
			 	{
					*shared = *shared+12;
					printf("[%d] Parent: Critical Section (%2d).\n",i+1, *shared);
				}
				wait(NULL);
				wait(NULL);
		}
    }
}
