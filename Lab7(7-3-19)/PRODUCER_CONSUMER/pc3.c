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
	int id=(int)args;
	int item;
	do
	{
		sem_wait(&nfull);      //consumer wait if nfull==0 i.e. when no item in queue
		pthread_mutex_lock(&mut);
		item=queue[front]; 
		front=(front+1) % n;
		printf("consumed product: %d by consumer-%d\n", item,id);
		pthread_mutex_unlock(&mut);
		sem_post(&nempty);/* tells inc in  free spaces in the queue*/
		sleep(rand()%4);
	}while(item);
}
void * produce(void * args)
{
	int item;
	//int id=(int)args;
	do
	{
		sem_wait(&nempty);       //producer wait for empty spaces i.e. if nempty=0 then there is no empty space so producer will wait
		//pthread_mutex_lock(&mut);
		item=rand()%100;
		printf("produced product : %d\n",item);
		queue[rear]=item; 
		rear=(rear+1)%n;
	//	pthread_mutex_unlock(&mut);
		sem_post(&nfull);        //producer signals that nfull is increased i.e. item is addded in queue
		sleep(rand()%4); 
	}while(item);
}
int main()
{
	pthread_t consumer[7],producer;
	sem_init(&nfull,0,0);                   
	sem_init(&nempty,0,n);   // zero means sharing b/w thread
	
	int i=0;
	
	pthread_create(&producer,NULL,&produce,NULL);
	for(i=1;i<=7;i++)
	{
		pthread_create(&consumer[i],NULL,&consume,(void *)i);		
	}
	
	
	pthread_join(producer,NULL);
	for(i=1;i<=7;i++)
	{
			pthread_join(consumer[i],NULL);		
	}
	
	sem_destroy(&nfull);
	sem_destroy(&nempty);
	return 0;
}
