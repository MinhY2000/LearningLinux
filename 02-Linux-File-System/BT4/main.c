#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

int main()
{
    const char *fileName = "text.txt";
    const char *data = "This's a piece of data written on file";
    int lengthData = strlen(data);
    int fd; // File Descriptor
    fd = open(fileName, O_RDWR);

    write(fd, data, lengthData);
    close(fd);

    struct stat statFile;
    if (stat(fileName, &statFile) == -1)
    {
        printf("Get info file is faild");
        return 1;
    }
    const char *fileStyle = S_ISDIR(statFile.st_mode) ? "Directory" : "File";
    char lastModifiedTime[100];
    strftime(lastModifiedTime, sizeof(lastModifiedTime), "%d-%m-%Y %H:%M:%S", localtime(&statFile.st_mtime));

    // In ra các thông tin về file
    printf("Type : %s\n", fileStyle);
    printf("Name : %s\n", fileName);
    printf("Last modified: %s\n", lastModifiedTime);
    printf("Size: %ld bytes\n", statFile.st_size);

    return 0;

}