#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<semaphore.h>
#include<pthread.h>
#define n1 7
#define n2 7
int r,data;
sem_t access_resource,access_read,service_queue;

void * read_fun(void *a)
{
    int id=(int)a;
    sem_wait(&service_queue);
    sem_wait(&access_read);
    if(r==0)
    {
    	sem_wait(&access_resource);
    }
    r++;
    sem_post(&service_queue);
    sem_post(&access_read);
    printf("THE VALUE READ BY %d READER :: %d\n",id,data);
    sem_wait(&access_read);
    r--;
    if(r==0)
    {
    	sem_post(&access_resource);
    }
    sem_post(&access_read);
    sleep(rand()%4);
}

void * write_fun(void *a)
{
    int id=(int)a;
    sem_wait(&service_queue);
    sem_wait(&access_resource);
    sem_post(&service_queue);
    data=data*2+1;
    printf("THE VALUE WRITTEN BY %d WRITER :: %d\n",id,data);
    sem_post(&access_resource);
    sleep(rand()%4);
}

int main()
{
    pthread_t reader[n1],writer[n2];
    sem_init(&access_resource,0,1);
    sem_init(&access_read,0,1);
    sem_init(&service_queue,0,1);
    int i;
    for(i=1;i<=n2;i++)
    {
        pthread_create(&writer[i],NULL,&write_fun,(void *)i);
        pthread_create(&reader[i],NULL,&read_fun,(void *)i);
    }
   /* for(i=1;i<=n1;i++)
    {
        pthread_create(&reader[i],NULL,&read_fun,(void *)i);
    }*/
    for(i=1;i<=n2;i++)
    {
        pthread_join(writer[i],NULL);
        pthread_join(reader[i],NULL);
    }
   /* for(i=1;i<=n1;i++)
    {
        pthread_join(reader[i],NULL);
    }*/
    return 0;
}
