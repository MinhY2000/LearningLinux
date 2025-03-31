#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int count = 0; 

void handle_signal(int sig) 
{
    if (sig == SIGUSR1) 
    {
        printf("Received signal from parent\n");
    }
}

int main() {
    int pid = fork();  

    if (pid == 0) 
    {
        signal(SIGUSR1, handle_signal);  
        while (1)
        {
            pause();  
        }
    }else if (pid > 0) 
    {
        while (count < 5) 
        {
            sleep(2); // em có 1 thắc mắc là khi em để sleep xuống dòng 34 thì process con không nhận được tín hiệu lun ạ
            kill(pid, SIGUSR1);  
            printf("Sent signal to child\n");
            count++;
        }
    }else
    {
        perror("fork failed");
        return 1;
    }
    //kill() là cách chuẩn để gửi tín hiệu giữa các process trong Linux. Mặc dù tên hàm có vẻ như dùng để kết thúc process, 
    //nhưng thực tế kill() chỉ đơn giản là gửi tín hiệu đến một process, và tín hiệu có thể là bất kỳ tín hiệu nào, 
    //không chỉ là tín hiệu để kết thúc một process.
    //Trong trường hợp này, kill() được sử dụng để gửi tín hiệu SIGUSR1 từ process cha tới process con. 
    //Nó là cách duy nhất trong trường hợp này để một process (cha) có thể gửi tín hiệu đến một process khác (con).
    
    return 0;
}
