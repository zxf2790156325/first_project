#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>


void * myfunc(void *arg)
{
    printf("child thread id :%lu\n", pthread_self());
    return NULL;
}

int main(void)
{
    pthread_t pthid;
    pthread_create(&pthid, NULL, myfunc, NULL);
    printf("parent thread id:%lu\n", pthread_self());

    for(int i = 0;i < 5;i++)
    {
        printf("i = %d\n", i);
    }


    sleep(1);
    return 0;
}

