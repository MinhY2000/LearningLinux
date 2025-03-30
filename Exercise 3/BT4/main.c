#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() 
{
    int returnValue;
    returnValue = fork();
    if (returnValue == 0) 
    {
        printf("Process Start....\n");
        sleep(2);
        printf("Process End....\n");
        exit(10);
    } else if (returnValue > 0) 
    {
        int status;
        wait(&status);
        if (WIFEXITED(status)) 
        {
            printf("Tiến trình con kết thúc với mã thoát %d\n", WEXITSTATUS(status));
        } else 
        {
            printf("Tiến trình con kết thúc không bình thường\n");
        }
    }else
    {
        perror("fork failed");
        return 1;
    }
    return 0;
}