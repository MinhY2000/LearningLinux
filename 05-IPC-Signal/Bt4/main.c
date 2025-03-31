#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int count = 0; 

void handle_signal(int sig) 
{
    printf("SIGTSTP ignored\n");
}
int main ()
{
    signal(SIGTSTP,handle_signal);
    while (1)
    {
        printf("Propramming is running...\n");
        sleep(1);
    }

    //Khi không xử lý tín hiệu SIGTSTP, và người dùng nhấn Ctrl+Z, chương trình sẽ bị dừng lại (pause)
    return 0;
}