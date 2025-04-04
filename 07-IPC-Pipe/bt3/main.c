#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fds[2];
    char write_msg[] = "I'm from Dong Nai city where have many special fruit.\n";
    char read_msg[100];
    int pid;

    if (pipe(fds) < 0)
    {
        printf("pipe() Unsuccessfully\n");
    }
    pid = fork();
    if (pid == -1)
    {
        printf("fork() Unsuccessfully\n");
    }
    if (pid > 0)
    {
        close(fds[0]);// 0 read, 1 write
        write(fds[1], write_msg, strlen(write_msg) + 1);
        close(fds[1]);
    }else
    {
        read(fds[0], read_msg, sizeof(read_msg) + 1);
        printf("Process child that read the data is : %s \n", read_msg);
        printf("Number of symbol in data:  %ld \n", strlen(read_msg));
        close(fds[0]);
    }
    return 0;

}