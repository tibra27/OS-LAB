#include<stdio.h>
#include<semaphore.h>
#include<stdlib.h>
#include<pthread.h>
#define n 100  
sem_t nfull, nempty;   //nfull==no. of items in queue//nempty=no. of empty spaces in queue
int queue[n];
int front,rear;
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
		sleep(rand()%2);
	}while(item);
}
void * produce(void * args)
{
	int item;
	do
	{
		item=rand()%100;
		printf("produced product : %d\n",item);
		sem_wait(&nempty);       //producer wait for empty spaces i.e. if nempty=0 then there is no empty space so producer will wait
		queue[rear]=item; 
		rear=(rear+1)%n;
		sem_post(&nfull);        //producer signals that nfull is increased i.e. item is addded in queue
		sleep(rand()%2); 
	}while(item);
}
int main()
{
	pthread_t consumer,producer;
	sem_init(&nfull,0,0);                   
	sem_init(&nempty,0,n);   // zero means sharing b/w thread
	
	//	int a,b;
//	sem_getvalue(&not_full,&a);
//	sem_getvalue(&not_empty,&b);
//	printf("a:%d  b%d  \n",a,b);
	
	pthread_create(&producer,NULL,&produce,NULL);
	pthread_create(&consumer,NULL,&consume,NULL);
	
	pthread_join(producer,NULL);
	pthread_join(consumer,NULL);
	
	sem_destroy(&nfull);
	sem_destroy(&nempty);
	return 0;
}
