#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

void* myfunc(void* arg)
{
    printf("chuld thread id:%lu\n", pthread_self());
    return NULL;
}

int main(void)
{
    pthread_t pthid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdatechstate(&attr, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&pthid, &attr, myfunc, NULL);
    if(ret != 0)
    {
        printf("error number:%d\n", ret);
        printf("%s\n", strerror(ret));
    }
    printf("parent thread id:%lu\n", pthread_self());
    for(int i = 0;i < 5;i++)
    {
        printf("i = %d\n", i);
    }
    sleep(2);
    pthread_attr_destory(&attr);
    return 0;




}
