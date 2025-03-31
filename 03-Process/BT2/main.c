#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        printf("Usage: %s [1/2]\n", argv[0]);
        return 1;
    }
    char mode = argv[1][0];
    int returnValue;
    returnValue = fork();
    if (mode == '1')
    {
        printf("Value environment = 1\n");
        setenv("COMMAND","1",1);
    }else if (mode == '2')
    {
        printf("Value environment = 2\n");
        setenv("COMMAND","2",1);
    }
    char *cmd[] = {"date",NULL};
    if (returnValue == 0) 
    {
        // // This is child Process
        // printf("Process child: My PID is %d\n", getpid());
        // printf("Child Process's running by 'ls'...\n");
        // execlp("ls", "ls", "-l", NULL);

        char *command = getenv("COMMAND");
        if (command[0] == '1')     
        {
            printf("Child Process's running by 'ls'...\n");
            execlp("ls", "ls", "-l", NULL);
        }
        else if (command[0] == '2')
        {
            printf("Child Process's running by 'date'...\n");
            execvp("date", cmd);
        }
        
        
    } else if (returnValue > 0) 
    {
        // This is Parent Process
        printf("Process parent: My child's PID is %d\n", returnValue);
    }else
    {
        perror("fork failed");
        return 1;
    }
    return 0;
}
// sau khi chạy hàm exec tiến trình con sẽ bị thay thế hoàn toàn bởi chương trình mới (ví dụ: ls hoặc date) 
//và mã tiếp theo trong tiến trình con sẽ không được thực thi nữa.
