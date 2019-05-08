#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<semaphore.h>
#include<pthread.h>
#define n1 7
#define n2 7
int r,data,w;
sem_t resourse,read_mutex,write_mutex,read_try;

void * read_fun(void *a)
{
    int id=(int)a;
    sem_wait(&read_try);
    sem_wait(&read_mutex);
    r++;
    if(r==1)
    {
        sem_wait(&resourse);
    }
    sem_post(&read_mutex);
    sem_post(&read_try);

    printf("THE VALUE READ BY %d READER :: %d\n",id,data);

    sem_wait(&read_mutex);
    r--;
    if(r==0)
    {
        sem_post(&resourse);
    }
    sem_post(&read_mutex);
    sleep(rand()%4);
}

void * write_fun(void *a)
{
    int id=(int)a;
    sem_wait(&write_mutex);
    w++;
    if(w==1)
    {
    	sem_wait(read_try);
    }
    sem_post(write_mutex);
    
    sem_wait(&resourse);
    data=data*2+1;
    printf("THE VALUE WRITTEN BY %d WRITER :: %d\n",id,data);
    sem_post(&resourse);
    
    sem_wait(&write_mutex);
    w--;
    if(w==0)
    {
    	sem_post(&read_try);
    }
    sem_post(&write_mutex);
    sleep(rand()%4);
}

int main()
{
    pthread_t reader[n1],writer[n2];
    sem_init(&resourse,0,1);
    sem_init(&read_mutex,0,1);
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
