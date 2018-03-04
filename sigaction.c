#include <stdio.h>
#include <signal.h>
#include <unistd.h>
void myfun(int no)
{
    printf("Hello World!\n");
    sleep(3);
    printf("wake up\n");
}

int main(void)
{
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT);
    act.sa_handler = myfun;
    sigaction(SIGINT, &act, NULL);
    
    while(1);

    return 0;
}
