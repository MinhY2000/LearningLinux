#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h> 

#define QUEUE_NAME "/my_queue"  
#define MAX_SIZE 1024         

// Cấu trúc tin nhắn
struct mq_msg 
{
    long mtype;          
    char mtext[MAX_SIZE]; 
};

void parent_process() 
{
    mqd_t mq;
    struct mq_msg msg;
    pid_t pid;

    // Cấu trúc để thiết lập các thuộc tính của message queue
    struct mq_attr attr;
    attr.mq_flags = 0;              // Không có cờ đặc biệt
    attr.mq_maxmsg = 10;            // Số lượng tin nhắn tối đa trong queue
    attr.mq_msgsize = sizeof(msg);  // Kích thước tối đa của một tin nhắn
    attr.mq_curmsgs = 0;            // Số tin nhắn hiện có trong queue (không cần thiết)

    // Tạo message queue với các thuộc tính đã định nghĩa
    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    if (mq == (mqd_t)-1) 
    {
        printf("Create queue fail\n");
        exit(1);
    }

    msg.mtype = 1;  
    strcpy(msg.mtext, "Hello Dad today I'm feel tired.\n"); 
    if (mq_send(mq, (const char *)&msg, sizeof(msg), 0) == -1) 
    {
        printf("Send data to child fail\n");
        exit(1);
    }

    wait(&pid);

    mq_close(mq);
    mq_unlink(QUEUE_NAME);
}

void child1_process() 
{
    mqd_t mq;
    pid_t pid;
    struct mq_msg msg;

    mq = mq_open(QUEUE_NAME, O_RDWR);
    if (mq == (mqd_t)-1) 
    {
        printf("open queue fail\n");
        exit(1);
    }

    if (mq_receive(mq, (char *)&msg, sizeof(msg), NULL) == -1) 
    {
        printf("Child receive fail\n");
        exit(1);
    }
    printf("Child 1 received message: %s\n", msg.mtext);
    for (int i = 0; msg.mtext[i] != '\0'; i++) 
    {
        msg.mtext[i] = toupper(msg.mtext[i]);
    }
    if (mq_send(mq, (const char *)&msg, sizeof(msg), 0) == -1) 
    {
        printf("Send data to child fail\n");
        exit(1);
    }
    printf("Child 1 send message: %s\n", msg.mtext);
    wait(&pid);
    mq_close(mq);
}
void child2_process() 
{
    mqd_t mq;
    struct mq_msg msg;

    mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == (mqd_t)-1) 
    {
        printf("open queue fail\n");
        exit(1);
    }

    if (mq_receive(mq, (char *)&msg, sizeof(msg), NULL) == -1) 
    {
        printf("Child receive fail\n");
        exit(1);
    }
    printf("Child 2 received message: %s\n", msg.mtext);
    mq_close(mq);
}

int main() 
{
    pid_t pid1, pid2 ;

    pid1 = fork();
    if (pid1 == -1) 
    {
        printf("fork() fail\n");
        exit(1);
    } else if (pid1 == 0) 
    {
        child1_process();
        exit(0);  // tiến trình con kết thúc một cách thành công. 
        //Mã trả về 0 là một cách chuẩn để thông báo rằng tiến trình con không gặp lỗi và hoàn thành nhiệm vụ của mình.
    }
    pid2 = fork();
    if (pid2 == -1) 
    {
        printf("fork() fail\n");
        exit(1);
    } else if (pid2 == 0) 
    {
        child2_process();
        exit(0);
    }
    parent_process();
    return 0;
}
