
#include <stdio.h>

#include <pthread.h>

int main()
{
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);//初始化后线程属性变量就包含了多种属性初始值

	//获取与设置分离属性
	int state = 0;
	pthread_attr_getdetachstate(&attr,&state);
	if(state == PTHREAD_CREATE_JOINABLE)
		printf("detachstate = %s\n","PTHREAD_CREATE_JOINABLE");
	else if(state == PTHREAD_CREATE_DETACHED)
		printf("detachstate = %s\n","PTHREAD_CREATE_DETACHED");	

	//获取与设置线程竞争范围
	int scope = 0;
	pthread_attr_getscope(&attr,&scope);
	if(scope == PTHREAD_SCOPE_SYSTEM)//系统范围
		printf("getscope = %s\n","PTHREAD_SCOPE_SYSTEM");
	else if(scope == PTHREAD_SCOPE_PROCESS)//进程范围
		printf("getscope = %s\n","PTHREAD_SCOPE_PROCESS");

	//获取与设置调度策略
	int policy = 0;
	pthread_attr_getschedpolicy(&attr, &policy);
	if(policy == SCHED_FIFO)
		printf("getschedpolicy = %s\n","SCHED_FIFO");	//如果线程有相同的优先级就按照先来先服务
	else if(policy == SCHED_RR)
		printf("getschedpolicy = %s\n","SCHED_RR");		//如果线程有相同的优先级，根据抢占的原则调度
	else	if(policy == SCHED_OTHER)
		printf("getschedpolicy = %s\n","SCHED_OTHER");  //

	//获取与设置线程栈大小
	size_t  stacksize = 0;
	pthread_attr_getstacksize(&attr,&stacksize);
	printf("stacksize:%lu\n",stacksize);			    //在这64位机上是 8M


	//获取与设置是否继承调度策略
	int inheritsched = 0;
	pthread_attr_getinheritsched(&attr,&inheritsched);
	if(inheritsched == PTHREAD_INHERIT_SCHED)
		printf("inheritsched = %s\n","PTHREAD_INHERIT_SCHED");
	else if(inheritsched == PTHREAD_EXPLICIT_SCHED)
		printf("inheritsched = %s\n","PTHREAD_EXPLICIT_SCHED");

	//获取与设置调度参数
	sched_param param;
	pthread_attr_getschedparam(&attr,&param);
	printf("sched_priority:%d\n", param.sched_priority);

	pthread_attr_destroy(&attr);
	
	int level;
	level = pthread_getconcurrency();
	printf("level:%d\n",level);
	
return 0;
}


