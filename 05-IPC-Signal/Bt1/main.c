#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Biến đếm số lần tín hiệu SIGINT được nhận
int sigint_count = 0;

// Hàm xử lý tín hiệu SIGINT
void handle_signal(int sig) 
{
    sigint_count++;
    printf("SIGINT received\n");
    if (sigint_count == 3) 
    {
        printf("Program terminating after 3 SIGINT signals.\n");
        exit(0); 
    }
}

int main() 
{
    signal(SIGINT, handle_signal);

    printf("Program running. Press Ctrl+C to send SIGINT.\n");


    while (1) 
    {
        // Chương trình sẽ không làm gì trong vòng lặp này
        // Chỉ chờ tín hiệu SIGINT từ người dùng
    }

    return 0;

    //Nếu bạn bỏ qua tín hiệu SIGINT, tức là không đăng ký bất kỳ hàm xử lý nào cho tín hiệu này, thì khi nhấn Ctrl+C, chương trình sẽ bị dừng ngay lập tức. 
    //Đây là hành động mặc định của tín hiệu SIGINT trong hầu hết các chương trình Unix/Linux.

}
