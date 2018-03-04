#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

int i = 100;

void *myfun(void *arg)
{
    int num = (int)arg;
    
    printf("%dth child thread id:%lu\n", num, pthread_self());
    pthread_exit(&i);
    return NULL;
}



int main(void)
{
    pthread_t pthid;
    
    
        pthread_create(&pthid, NULL, myfun, (void *)i);
    

    printf("parent thread id:%lu\n", pthread_self());
    for(int i = 0;i < 5; i++)
    {
        printf("i = %d\n", i);
    }


    void *ptr = NULL;
    pthread_join(pthid, &ptr);
    printf("ptr = %d\n",*(int *)ptr);
    return 0;
}
