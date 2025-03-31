#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void main()
{
    int fd; // File Descriptor
    int number_W, number_W1; // number byte Write
    fd = open("text.txt", O_APPEND | O_WRONLY);
    // fd = open("text.txt", O_WRONLY);
    const char *buff = "I'm so happy because the speaking English is improving day by day\n";
    const char *buff1 = "Really That's amazing\n";
    if (fd == -1)
    {
        printf("Open text.txt failed\n");
    }
    number_W1 = write(fd, buff1, strlen(buff1));
    printf("Write %d bytes to file text.txt\n", number_W1);
    lseek(fd,0,SEEK_SET);
    number_W = write(fd, buff, strlen(buff));
    printf("Write %d bytes to file text.txt\n", number_W);
    close(fd);
}