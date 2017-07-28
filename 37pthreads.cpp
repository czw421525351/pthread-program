
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>

#include <pthread.h>


#define EXIT(m) \
					do \
					{ \
						perror(m); \
						exit(EXIT_FALURE); \
					}while(0)

void* thread(void* args)
{
		int i;
		for(i = 0; i < 20 ; ++i)
		{
			printf("B");
			fflush(stdout);
			usleep(1000);
			// if(i == 3)
				// pthread_exit("123");
		}
		return (void*)"456";
}

int main()
{
	pthread_t threadid;
	int ret = 0;
	if((ret = pthread_create(&threadid,NULL,thread,NULL)) != 0)
	{					 
		fprintf(stderr,"pthread_create %s\n",strerror(ret));
		exit(EXIT_FAILURE);//不用 EXIT(); 因为里面用到了perror
		
	}
	int i;
	for(i = 0; i < 20 ; ++i)
	{
		printf("A");
		fflush(stdout);
		usleep(1000);
	}
	void* retstat;
	if((ret = pthread_join(threadid,&retstat)) != 0 )
	{
		fprintf(stderr,"pthread_join %s\n",strerror(ret));
		exit(EXIT_FAILURE);
	}
	printf("thread return state %s\n",(char*)retstat);
	
return 0;
}