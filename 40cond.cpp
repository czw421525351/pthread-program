
#include<stdio.h>
#include <string.h>

#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define CONSUME_COUNT 5
#define PRODUCE_COUNT 3 

pthread_t threadIds[CONSUME_COUNT + PRODUCE_COUNT] = {0};
pthread_mutex_t g_mutex ;//= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_cond;

int nready = 0;

void* consume_routine(void* args)
{
	unsigned long threadno = (unsigned long)args;
	while(1)
	{
		pthread_mutex_lock(&g_mutex);
		
		while(nready == 0)//为什么要用while循环
		{
			printf("consume thread%ld waiting cond\n",threadno);
			pthread_cond_wait(&g_cond,&g_mutex);
			printf("consume thread%ld has gotten cond\n",threadno);
		}
		--nready;
		printf("consume thread%ld has comsune produce\n\n",threadno);
		pthread_mutex_unlock(&g_mutex);
		sleep(1);
	}
	return NULL;
}

void* produce_routine(void* args)
{
	unsigned long threadno = (unsigned long)args;
	while(1)
	{
		pthread_mutex_lock(&g_mutex);
		
		printf("produce thread%ld producting cond\n",threadno);
		++nready;
		printf("produce thread%ld has producted cond\n\n",threadno);
		pthread_cond_signal(&g_cond);
		pthread_mutex_unlock(&g_mutex);
		sleep(3);
	}
	return NULL;
}

int main()
{

	pthread_mutex_init(&g_mutex,NULL);
	pthread_cond_init(&g_cond,NULL);
	
	int i = 0;
	for(;i < CONSUME_COUNT ; ++i)
		pthread_create(threadIds+i , NULL , consume_routine,(void*)i);
	
	for(i = 0;i < PRODUCE_COUNT ; ++i)
		pthread_create(threadIds+CONSUME_COUNT+i , NULL , produce_routine,(void*)i);
	
	for(i = 0; i < (CONSUME_COUNT + PRODUCE_COUNT); ++i)
		pthread_join(threadIds[i],NULL);
	
	pthread_mutex_destroy(&g_mutex);
	pthread_cond_destroy(&g_cond);
	
	return 0;
}


