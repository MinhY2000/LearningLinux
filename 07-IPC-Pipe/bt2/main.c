#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int pipe_Dadto1[2], pipe_1to2[2];
    char msg_from_dad[] = "Hello I'm Tran Minh Y.";
    char msg_from_child1[100];
    char msg_from_child2[100];
    int pid1, pid2;

    if (pipe(pipe_Dadto1) || pipe(pipe_1to2)  < 0)
    {
        printf("pipe() Unsuccessfully\n");
    }
    pid1 = fork();
    if (pid1 == -1)
    {
        printf("fork() Child 1 Unsuccessfully\n");
    }
    if (pid1 > 0)
    {
        close(pipe_Dadto1[0]);// 0 read, 1 write
        write(pipe_Dadto1[1], msg_from_dad, strlen(msg_from_dad) + 1);
        close(pipe_Dadto1[1]);
    }else
    {
        pid2 = fork();
        if (pid2 == -1)
        {
            printf("fork() Child 2 Unsuccessfully\n");
        }
        if (pid2 > 0)
        {
            read(pipe_Dadto1[0], msg_from_child1, sizeof(msg_from_child1) + 1);
            printf("\nData C1: %s \n", msg_from_child1);
            close(pipe_Dadto1[0]);
            strcat(msg_from_child1, " I'm 24 year olds and I graduated HCMUT\n");
            close(pipe_1to2[0]);// 0 read, 1 write
            write(pipe_1to2[1], msg_from_child1, strlen(msg_from_child1) + 1);
            close(pipe_1to2[1]);
        }else
        {
            read(pipe_1to2[0], msg_from_child2, sizeof(msg_from_child2) + 1);
            printf("\nData C2: %s \n", msg_from_child2);
            close(pipe_1to2[0]);
        }
    }
    return 0;

}