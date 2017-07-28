
#include<stdio.h>
#include <string.h>

#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define CONSUME_COUNT 10 
#define PRODUCE_COUNT 20 
#define BUFFER_SIZE   10

int buffer[BUFFER_SIZE] = {0};
pthread_t threadIds[CONSUME_COUNT + PRODUCE_COUNT] = {0};

sem_t full;
sem_t empty;
pthread_mutex_t buffert_mutex ;//= PTHREAD_MUTEX_INITIALIZER;

unsigned short in = 0;
unsigned short out = 0;
unsigned short produce_id = 0; 
unsigned short consume_id = 0; 

void* consume_routine(void* args)
{
	int i = 0;
	unsigned long threadno = (unsigned long)args;
	while(1)
	{
		printf("\nconsume thread%d wait for sem empty\n\n",threadno);	

		sem_wait(&empty);

		pthread_mutex_lock(&buffert_mutex);

		for(i = 0; i < BUFFER_SIZE ; ++i)//打印仓库当前的状态
		{
			if(buffer[i] == -1)
				printf("consume thread%d : buffer[%d] = NULL\n",threadno,i);
			else
				printf("consume thread%d : buffer[%d] = %d\n",threadno,i,buffer[i]);
		}

		consume_id = buffer[out];
		buffer[out] = -1;
		printf("\nconsume thread%d consume %d at buffer[%d]\n\n",threadno,consume_id,out);	
		out = (++out)%BUFFER_SIZE;
		
		pthread_mutex_unlock(&buffert_mutex);	

		sem_post(&full);

		sleep(1);
	}
	return NULL;
}

void* produce_routine(void* args)
{
	int i ;
	unsigned long threadno = (unsigned long)args;
	while(1)
	{
		printf("\nproduce thread%d wait for sem full\n\n",threadno);	

        sem_wait(&full);

        pthread_mutex_lock(&buffert_mutex);

        for(i = 0; i < BUFFER_SIZE; ++i)//打印仓库当前的状态
		{
			if(buffer[i] == -1)
				printf("produce thread%d : buffer[%d] = NULL\n",threadno,i);
			else
				printf("produce thread%d : buffer[%d] = %d\n",threadno,i,buffer[i]);
		}

		buffer[in] = produce_id;
		printf("\nproduce thread%d produce %d at buffer[%d]\n\n",threadno,produce_id,in);	
		++produce_id;
		in = (++in)%BUFFER_SIZE;
		
		pthread_mutex_unlock(&buffert_mutex);

        sem_post(&empty);
		
		sleep(2);
	}
	return NULL;
}

int main()
{
	memset(buffer,-1,sizeof(buffer));

	sem_init(&full,0,BUFFER_SIZE);//初始化为10
	sem_init(&empty,0,0);		  //初始化为0

	pthread_mutex_init(&buffert_mutex,NULL);//初始化互斥锁
	
	int i = 0;
	for(;i < CONSUME_COUNT ; ++i)
		pthread_create(threadIds+i , NULL , consume_routine,(void*)i);
	
	for(i = 0;i < PRODUCE_COUNT ; ++i)
		pthread_create(threadIds+CONSUME_COUNT+i , NULL , produce_routine,(void*)i);
	
	for(i = 0; i < (CONSUME_COUNT + PRODUCE_COUNT); ++i)
		pthread_join(threadIds[i],NULL);
	
	sem_destroy(&full);
	sem_destroy(&empty);
	pthread_mutex_destroy(&buffert_mutex);
	return 0;
}


