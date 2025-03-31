#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int timer = 0;

void handle_alarm(int sig)
 {
    timer++;  
    printf("Timer: %d seconds\n", timer);

    if (timer == 10)
    {
        printf("Timer finished.\n");
        exit(0); 
    }
    alarm(1);
}

int main() 
{
    signal(SIGALRM, handle_alarm);
    alarm(1);

    while (1) 
    {
        // Chương trình chỉ chờ nhận tín hiệu SIGALRM
        // Dừng chương trình chờ tín hiệu
    }
    //Nếu bạn không gọi lại alarm(1) trong hàm xử lý tín hiệu, thì sau khi nhận tín hiệu SIGALRM đầu tiên, chương trình sẽ không gửi thêm tín hiệu SIGALRM nữa. 
    //Điều này có nghĩa là bộ đếm thời gian sẽ chỉ chạy một lần và không tiếp tục tăng sau mỗi giây, vì không có tín hiệu mới được gửi sau lần đầu tiên.
    //Kết quả là chương trình chỉ đếm một giây và sau đó sẽ dừng mà không tiếp tục đếm.
    return 0;
}
