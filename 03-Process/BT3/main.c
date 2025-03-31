#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void signal_handler(int sig)
{
    if (sig == SIGUSR1) 
    {
        printf("Tiến trình con nhận được tín hiệu SIGUSR1\n");
    }
}

int main()
{
    int returnValue;
    returnValue = fork();
    if (returnValue == 0) 
    {
        signal(SIGUSR1, signal_handler);
        printf("Tiến trình con đang chờ tín hiệu SIGUSR1...\n");
        while (1) 
        {
            pause();  
        }
    }else if (returnValue > 0) 
    {
        sleep(2);
        kill(returnValue, SIGUSR1);
        printf("Tiến trình cha đã gửi tín hiệu SIGUSR1 tới tiến trình con\n");
    }else
    {
        perror("fork failed");
        return 1;
    }
    return 0;
}