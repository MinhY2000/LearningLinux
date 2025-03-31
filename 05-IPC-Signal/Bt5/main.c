#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <string.h>
#include <sys/time.h>


int sigint_received = 0;
int sigterm_received = 0;

void handle_sigint(int sig) 
{
    sigint_received = 1;  
}
void handle_sigterm(int sig) 
{
    sigterm_received = 1;  
}

int main() 
{

    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigterm);

    fd_set read_fds;
    struct timeval timeout;
    char input[100];

    while (1) 
    {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        timeout.tv_sec = 1;  
        timeout.tv_usec = 0;

        int ready = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);

        if (ready < 0) 
        {
            printf("select\n");
            if (sigint_received) 
            {
                printf("SIGINT received.\n");
                sigint_received = 0;  
            }
            if (sigterm_received) 
            {
                printf("SIGTERM received. Exiting...\n");
                sigterm_received = 0;
                break;  
            }
            // exit(1);
        } else if (ready > 0) 
        {

            if (FD_ISSET(STDIN_FILENO, &read_fds)) 
            {
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;  // Xóa ký tự newline
                printf("You entered: %s\n", input);
            }
        }
    }
    return 0;
}
