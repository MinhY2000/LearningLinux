#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[]) 
{
    if (argc != 4 && argc != 5) 
    {
        printf("Usage: %s filename num-bytes [r/w] \"Hello\"\n", argv[0]);
        return 1;
    }
    
    char *filename = argv[1];
    int numByte = atoi(argv[2]);
    char mode = argv[3][0]; //argv[3][0] là ký tự đầu tiên trong chuỗi đó, tức là bạn đang lấy giá trị ký tự 'r' từ chuỗi
    char *content = argv[4];

    int fd; // File Descriptor
    fd = open("text.txt", O_RDWR);
    char *buff = (char *)malloc(numByte + 1);  // Dự trữ bộ nhớ cho dữ liệu
    if (buff == NULL) 
    {
        perror("Error allocating memory");
        close(fd);
        return 1;
    }
    if (mode == 'r')
    {
        size_t bytes_read = read(fd, buff, numByte);
        printf("Read %zu bytes: %s\n", bytes_read, buff);
    }else if (mode == 'w')
    {
        size_t bytes_write = write(fd, content, numByte);
        printf("Write %zu bytes: %s\n", bytes_write, content);
    }else printf("Fail mode Read or Write");
    close(fd);
    return 0;
}
    