#include<stdio.h>
#include<semaphore.h>
#include<stdlib.h>
#include<pthread.h>
#define n 100  
sem_t nfull, nempty;   //nfull==no. of items in queue//nempty=no. of empty spaces in queue
int queue[n];
int front,rear;
pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;
void * consume(void * args)
{
	int item;
	do
	{
		sem_wait(&nfull);      //consumer wait if nfull==0 i.e. when no item in queue
		item=queue[front]; 
		front=(front+1) % n;
		printf("consumed product: %d\n", item);
		sem_post(&nempty);/* tells inc in  free spaces in the queue*/
		sleep(rand()%4);
	}while(item);
}
void * produce(void * args)
{
	int item;
	int id=(int)args;
	do
	{
		sem_wait(&nempty);       //producer wait for empty spaces i.e. if nempty=0 then there is no empty space so producer will wait
		pthread_mutex_lock(&mut);
		item=rand()%100;
		printf("produced product : %d by producer-%d\n",item,id);
		queue[rear]=item; 
		rear=(rear+1)%n;
		pthread_mutex_unlock(&mut);
		sem_post(&nfull);        //producer signals that nfull is increased i.e. item is addded in queue
		sleep(rand()%4); 
	}while(item);
}
int main()
{
	pthread_t consumer,producer[7];
	sem_init(&nfull,0,0);                   
	sem_init(&nempty,0,n);   // zero means sharing b/w thread
	
	int i=0;
	for(i=1;i<=7;i++)
	{
		pthread_create(&producer[i],NULL,&produce,(void *)i);		
	}
	pthread_create(&consumer,NULL,&consume,NULL);
	
	for(i=1;i<=7;i++)
	{
			pthread_join(producer[i],NULL);		
	}
	pthread_join(consumer,NULL);
	
	sem_destroy(&nfull);
	sem_destroy(&nempty);
	return 0;
}
