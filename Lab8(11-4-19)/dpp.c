#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
sem_t forks[5];
sem_t mutex;

int left(int p)
{
	return p;
}
int right(int p)
{
	return((p+1)%5);
}

void think(int i)
{
	printf("%d-PHILOSOPHER IS THINKING!!!\n",i);
}
void eat(int i)
{
	printf("%d-PHILOSOPHER IS EATING!!!\n",i);
}
void putforks(int p)
{
	sem_post(&forks[left(p)]);
	printf("%d-PHILOSOPHER RELEASE CHOPSTICK NO.-%d\n",p,left(p));
	sem_post(&forks[right(p)]);
	printf("%d-PHILOSOPHER RELEASE CHOPSTICK NO.-%d\n",p,right(p));
}
void getforks(int p)
{
	if(p==4)
	{
		sem_wait(&forks[right(p)]);
		printf("%d-PHILOSOPHER GOT     CHOPSTICK NO.-%d\n",p,right(p));
		sem_wait(&forks[left(p)]);
		printf("%d-PHILOSOPHER GOT     CHOPSTICK NO.-%d\n",p,left(p));
	}
	else
	{
		sem_wait(&forks[left(p)]);
		printf("%d-PHILOSOPHER GOT     CHOPSTICK NO.-%d\n",p,left(p));
		sem_wait(&forks[right(p)]);
		printf("%d-PHILOSOPHER GOT     CHOPSTICK NO.-%d\n",p,right(p));
	}
}




void * phil_func(void *a)
{
	int n=(int)a;
	while(1)
	{
		sleep(1);
		think(n);
		sleep(1);
		//sem_wait(&mutex);  //so that no two philosopher get forks at same time
		getforks(n);
		//sem_post(&mutex);
		sleep(1);
		eat(n);
		sleep(1);
		putforks(n);
		
	}
}



int main()
{
	int i;
	pthread_t phil[5];
	for(i=0;i<5;i++)
	{
		sem_init(&forks[i],0,1);
	}
	sem_init(&mutex,0,1);
	for(i=0;i<5;i++)
	{
		pthread_create(&phil[i],NULL,phil_func,(void *)i);
	}
	for(i=0;i<5;i++)
	{
		pthread_join(phil[i],NULL);
	}
}
