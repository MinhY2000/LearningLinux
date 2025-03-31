#include <stdio.h>
#include <unistd.h>

int main() 
{
    int returnValue;
    returnValue = fork(); 

    if (returnValue == 0) 
    {
        // This is child Process
        printf("Process child: My PID is %d\n", getpid());
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