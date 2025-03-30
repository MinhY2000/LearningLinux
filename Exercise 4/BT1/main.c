#include <stdio.h>
#include <pthread.h>

void* print_message(void* thread_id)
{
    long id = (long) thread_id;
    printf("Thread %ld: Hello fromm Thread\n", id);
    return NULL;
}
int main()
{
    pthread_t thread1, thread2;
    long id1 = 1;
    long id2 = 2;

    pthread_create(&thread1, NULL, print_message, (void*)id1);
    pthread_create(&thread2, NULL, print_message, (void*)id2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Both threads have finished.\n");

    //Hàm pthread_create() được sử dụng để tạo một thread mới
    //Hàm pthread_join() được sử dụng để đợi một thread hoàn thành trước khi tiếp tục thực thi

    //Một thread kết thúc khi hàm start_routine mà nó thực thi hoàn thành. 
    //Trong trường hợp này, hàm print_message kết thúc khi nó in xong thông báo.
    
    return 0;
}