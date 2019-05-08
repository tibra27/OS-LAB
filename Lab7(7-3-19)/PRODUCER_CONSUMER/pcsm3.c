#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#define size 10

int	shmID1,shmID2,shmID3;
key_t 	key1 = 54991, key2 = 784519, key3 = 789456;
int	*queue, *front, *rear;


int main()
{
	srand(time(0));
	sem_t *nempty, *nfull,*mutex;
    shmID1 = shmget(key1, size * sizeof(int), IPC_CREAT | 0660);
    if ( shmID1 < 0 )
    {
        printf("Shared memory could not be allocated. Quitting.\n");
        exit(1);
    }

    shmID2 = shmget(key2, sizeof(int), IPC_CREAT | 0660);
    if ( shmID2 < 0 )
    {
        printf("Shared memory 1 could not be allocated. Quitting.\n");
        exit(1);
    }

    shmID3 = shmget(key3, sizeof(int), IPC_CREAT | 0660);
    if ( shmID3 < 0 )
    {
        printf("Shared memory 2 could not be allocated. Quitting.\n");
        exit(1);
    }


	queue = (int *)shmat(shmID1, NULL, 0);
    if (queue == (int *)-1 )
    {
        printf("Failed to attach queue to main process. Quitting.\n");
        exit(-1);
    }
    front = (int *)shmat(shmID2, NULL, 0);
    if (front == (int *)-1 )
    {
        printf("Failed to attach front to main process. Quitting.\n");
        exit(-1);
    }
    rear = (int *)shmat(shmID3, NULL, 0);
    if (rear == (int *)-1 )
    {
        printf("Failed to attach rear to main process. Quitting.\n");
        exit(-1);
    }

    nempty = sem_open("empty",O_CREAT | O_EXCL, 0644, size); // initialize semaphores for shared processes
    nfull = sem_open("full",O_CREAT | O_EXCL,0644, 0);
    mutex = sem_open("mutex_lock",O_CREAT | O_EXCL,0644, 1);       //mutex is used as semaphore

    sem_unlink("empty"); //  name of semaphore is "pSem", semaphore is reached using this name
    sem_unlink("full");
    sem_unlink("mutex_lock");
    printf ("semaphores initialized.\n\n");

	*front = 0;
	*rear = 0;


    // Create First (child) process  -- producer
    if (fork() == 0)
    {
    	queue = (int *)shmat(shmID1, NULL, 0);
        if (queue == (int *)-1 )
        {
            printf("Failed to attach queue to producer process. Quitting.\n");
            exit(-1);
        }
        front = (int *)shmat(shmID2, NULL, 0);
        if (front == (int *)-1 )
        {
            printf("Failed to attach front to producer process. Quitting.\n");
            exit(-1);
        }
        rear = (int *)shmat(shmID3, NULL, 0);
        if (rear == (int *)-1 )
        {
            printf("Failed to attach rear to producer process. Quitting.\n");
            exit(-1);
        }

	// This is the code first process executes

    	int item;
    	do
    	{
    		sem_wait(nempty);       //producer wait for empty spaces i.e. if nempty=0 then there is no empty space so producer will wait
    	    item=rand()%100;
    		printf("product produced : %d\n",item);
    		
    		queue[*rear]=item;
    		*rear=(*rear+1)%size;
    		sem_post(nfull);        //producer signals that nfull is increased i.e. item is addded in queue
    		sleep(rand()%2);
    	}while(item);

    }
    else
    {
        // Create second (child) process  -- consumer
        if (fork() == 0)
        {
        	fork();
        	fork();
        	fork();
            queue = (int *)shmat(shmID1, NULL, 0);
            if (queue == (int *)-1 )
            {
                printf("Failed to attach consumer to producer process. Quitting.\n");
                exit(-1);
            }

            front = (int *)shmat(shmID2, NULL, 0);
            if (front == (int *)-1 )
            {
                printf("Failed to attach consumer to producer process. Quitting.\n");
                exit(-1);
            }

            rear = (int *)shmat(shmID3, NULL, 0);
            if (rear == (int *)-1 )
            {
                printf("Failed to attach consumer to producer process. Quitting.\n");
                exit(-1);
            }


    	    // This is the code second process executes

            int item;
        	do
        	{
        		sem_wait(nfull);      //consumer wait if nfull==0 i.e. when no item in queue
        		sem_wait(mutex);
        		item=queue[*front];
        		*front=(*front+1) % size;
        		printf("product consumed: %d by consumer %d\n", item,getpid());
        		sem_post(mutex);
        		sem_post(nempty);/* tells inc in  free spaces in the queue*/
        		sleep(rand()%2);
        	}while(item);
        }
        else
        {
        	wait(NULL);
        	wait(NULL);
        	printf("All child process exited\n");
        	shmdt(queue);             ///* shared memory detach */
        	shmdt(nempty);
        	shmdt(nfull);
        	shmctl(shmID1,IPC_RMID, 0);
        	shmctl(shmID2,IPC_RMID, 0);
        	shmctl(shmID3,IPC_RMID, 0);
        	/* cleanup semaphores */

            sem_destroy (nempty);
            sem_destroy (nfull);
            sem_destroy (mutex);
        }
    }
}
