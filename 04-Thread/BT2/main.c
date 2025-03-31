#include <stdio.h>
#include <pthread.h>

#define NUM_THREAD 3
#define NUM_REPETITION 1000000

long counter = 0;

pthread_mutex_t counter_mutex;

void* print_message(void* thread_id)
{
    for (int i = 0; i < NUM_REPETITION; i++)
    {
        pthread_mutex_lock(&counter_mutex); 
        counter++;
        pthread_mutex_unlock(&counter_mutex);
    }
    
    return NULL;
}
int main()
{
    pthread_t thread[NUM_THREAD];
    pthread_mutex_init(&counter_mutex, NULL);

    for (long i = 0; i < NUM_THREAD; i++)
    {
        pthread_create(&thread[i], NULL, print_message, (void*)i);
    }
    for (int i = 0; i < NUM_THREAD; i++)
    {
        pthread_join(thread[i], NULL);
    }

    printf("Final counter value: %ld\n", counter);

    pthread_mutex_destroy(&counter_mutex);
    
    //Mutex là cần thiết để tránh tình trạng race condition
    //Nếu không sử dụng mutex, nhiều thread có thể truy cập và thay đổi giá trị của counter đồng thời. 
    //Điều này có thể dẫn đến việc mất đồng bộ, tức là các giá trị thay đổi không chính xác.
    
    //pthread_mutex_init: Khởi tạo mutex trước khi sử dụng.
    //pthread_mutex_destroy: Hủy mutex khi không còn cần thiết.
    return 0;
}