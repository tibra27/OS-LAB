#include<stdio.h>
#include<stdlib.h>  
#include<unistd.h>
#include<time.h>
#include<semaphore.h>
#include<sys/wait.h>
#include<sys/shm.h> 
#include<sys/types.h>  
#include<fcntl.h>     

int	shmID1,shmID2,shmID3; 
int	*readcount,*shared,*writecount;
sem_t *resource, *rmutex,*wmutex,*readtry;
key_t key1 = 5491, key2 = 784519, key3 = 789456;
int main()
{
    srand(time(0));
    
    shmID1=shmget(key1,sizeof(int),IPC_CREAT | 0660);
    if(shmID1 <0)
    {
        printf("Shared memory could not be allocated. Quitting.\n");
        exit(1);
    }
    shmID2=shmget(key2,sizeof(int),IPC_CREAT | 0660);
    if(shmID2 <0)
    {
        printf("Shared memory could not be allocated. Quitting.\n");
        exit(1);
    }
    shmID3=shmget(key3,sizeof(int),IPC_CREAT | 0660);
    if(shmID3 <0)
    {
        printf("Shared memory could not be allocated. Quitting.\n");
        exit(1);
    }

    readcount = (int *)shmat(shmID1, NULL, 0);
    if (readcount == (int *)-1 )
    {
        printf("Failed to attach readcount to main process. Quitting.\n");
        exit(-1);
    }

    shared = (int *)shmat(shmID2, NULL, 0);
    if (shared == (int *)-1 )
    {
        printf("Failed to attach shared to main process. Quitting.\n");
        exit(-1);
    }
    writecount = (int *)shmat(shmID3, NULL, 0);
    if (writecount == (int *)-1 )
    {
        printf("Failed to attach shared to main process. Quitting.\n");
        exit(-1);
    }

    resource=sem_open("resource",O_CREAT |O_EXCL,0644,1);
    rmutex=sem_open("read_mutex",O_CREAT |O_EXCL,0644,1);
    wmutex=sem_open("write_mutex",O_CREAT |O_EXCL,0644,1);
    readtry=sem_open("read_try",O_CREAT |O_EXCL,0644,1);


    sem_unlink("resource");
    sem_unlink("read_mutex");
    sem_unlink("write_mutex");
    sem_unlink("read_try");

    *readcount = 0;
	*shared = 1;
	*writecount=0;
	 //writer creation
    if(fork()==0) 
    {
        fork();           //multiple writers
    	fork();
    	fork();

    	readcount = (int *)shmat(shmID1, NULL, 0);
	    if (readcount == (int *)-1 )
	    {
	        printf("Failed to attach readcount to writer process. Quitting.\n");
	        exit(-1);
	    }

	    shared = (int *)shmat(shmID2, NULL, 0);
	    if (shared == (int *)-1 )
	    {
	        printf("Failed to attach shared to writer process. Quitting.\n");
	        exit(-1);
	    }
	    writecount = (int *)shmat(shmID3, NULL, 0);
		if (writecount == (int *)-1 )
		{
		    printf("Failed to attach shared to main process. Quitting.\n");
		    exit(-1);
		}

	    //writer code
	    sem_wait(wmutex);
	    *writecount++;
	    if(*writecount==1)
	    {
	    	sem_wait(readtry);
	    }
	    sem_post(wmutex);
	    sem_wait(resource);
	    *shared=*shared*2+1;
	    printf("%d written  by writer: %d\n",*shared,getpid());
	    sem_post(resource);
	    sem_wait(wmutex);
        *writecount--;
        if(*writecount == 0)
        {
            sem_post(readtry);
        }
        sem_post(wmutex);
	    sleep(rand() % 4);
    }
    else
    {
    	//reader creation
        if(fork()==0)  
        {
            fork();        //multiple readers
        	fork();
        	fork();

    	    readcount = (int *)shmat(shmID1, NULL, 0);
	        if (readcount == (int *)-1 )
	        {
    	        printf("Failed to attach readcount to reader process. Quitting.\n");
	            exit(-1);
	        }

	        shared = (int *)shmat(shmID2, NULL, 0);
	        if (shared == (int *)-1 )
	        {
	            printf("Failed to attach shared to reader process. Quitting.\n");
	            exit(-1);
	        }
	        writecount = (int *)shmat(shmID3, NULL, 0);
			if (writecount == (int *)-1 )
			{
				printf("Failed to attach shared to main process. Quitting.\n");
				exit(-1);
			}

	        //reader code
			sem_wait(readtry);
	        sem_wait(rmutex);
	        *readcount++;
	        if(*readcount==1)
	        {
	            sem_wait(resource);
	        }

	        sem_post(rmutex);
	        sem_post(readtry);
	        printf("%d read by reader: %d\n",*shared,getpid());
	        sem_wait(rmutex);
	        *readcount--;
	        if(*readcount==0)
	        {
	            sem_post(resource);
	        }
            sem_post(rmutex);

            sleep(rand() % 4);



        }
        else
        {
            wait(NULL);
            wait(NULL);

            shmdt(resource);
            shmdt(readtry);
            shmdt(rmutex);
            shmdt(wmutex);
            shmctl(shmID1,IPC_RMID, 0);
    	    
    	    shmctl(shmID2,IPC_RMID, 0);
    	    
    	    shmctl(shmID3,IPC_RMID, 0);

    	    /* cleanup semaphores */

            sem_destroy (rmutex);
            sem_destroy (resource);
            sem_destroy (wmutex);
            sem_destroy (readtry);
        }
    }
}
