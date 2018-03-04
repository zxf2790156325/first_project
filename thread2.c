#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

void *myfun(void *arg)
{
    int num = (int)arg;
    printf("%dth child thread id:%lu\n", num, pthread_self());
    return NULL;
}

int main(void)
{
    pthread_t pthid[5];
    for(int i = 0;i < 5;i++)
    {
        pthread_create(&pthid[i], NULL, myfun, (void *)i);
    }

    printf("parent thread id:%lu\n", pthread_self());
    for(int i = 0;i < 5; i++)
    {
        printf("i = %d\n", i);
    }



    sleep(2);
    return 0;
}
