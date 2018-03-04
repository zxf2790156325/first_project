#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

typedef struct node
{
    int date;
    struct node* next;
}Node;

Node* head = NULL;

pthread_mutex_t mutex;
pthread_cond_t cond;

void* producer(void* arg)
{
    while(1)
    {
        Node* pnew = (Node*)malloc(sizeof(Node));
        pnew->date = rand() % 1000;
        pthread_mutex_lock(&mutex);
        pnew->next = head;
        printf("==producer:%lu, %d\n", pthread_self(), pnew->date);
        pthread_mutex_unlock(&mutex);

        pthread_cond_signal(&cond);

        sleep(rand() % 3);
    }
    return NULL;
}

void* customer(void* arg)
{

    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(head == NULL)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        Node* pdel = head;
        head = head->next;
        printf("--customer:%lu, %d\n", pthread_self(), pdel->date);
        free(pdel);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


int main(void)
{
    pthread_t p1, p2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&p1, NULL, producer, NULL);
    pthread_create(&p2, NULL, customer, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
