//
// Created by 17723 on 2017/5/15.
//
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>

typedef struct tsd
{
    pthread_t tid;
    char *str;
}tsd_t;

pthread_key_t  key_tsd;
pthread_once_t once_control = PTHREAD_ONCE_INIT;

void destory_routine(void* value)//会执行两次
{
    printf("destory...\n");
    free(value);
}

void key_create(void)
{
    pthread_key_create(&key_tsd,destory_routine);//提供一个函数指针用于销毁数据
    printf("key_create...\n");
}

void* thread_routine(void* args)
{
    pthread_once(&once_control,key_create);

    tsd_t *value = (tsd_t*)malloc(sizeof(tsd_t));
    value->tid = pthread_self();
    value->str = (char*)args;
    pthread_setspecific(key_tsd,value);//设定特定数据
    printf("%s setspecific %p\n",(char*)args,value);
    value = (tsd_t*)pthread_getspecific(key_tsd);
    printf("tid=0x%x, str = %s\n",value->tid,value->str);
    sleep(2);
    printf("tid=0x%x, str = %s\n",value->tid,value->str);

    return NULL;
}


int main()
{
    pthread_t tid1;
    pthread_t tid2;

    pthread_create(&tid1,NULL,thread_routine,(void *)"thread1");
    pthread_create(&tid2,NULL,thread_routine,(void *)"thread2");

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    pthread_key_delete(key_tsd);
    return 0;

}