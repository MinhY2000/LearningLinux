#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define Array_size 1000000
#define NUM_THREADS 4

int arr[Array_size];
long long total_sum = 0;

pthread_mutex_t sum_mutex;

void Create_random_array()
{
    for (int i = 0; i < Array_size; i++)
    {
        arr[i] = rand() % 100 + 1;
    }
}
void* sum(void* arg)
{
    int thread_ids = *((int*)arg);
    int start = (Array_size/NUM_THREADS)*thread_ids;
    int end = (Array_size/NUM_THREADS)*(thread_ids + 1);
    long s = 0;
    for (int i = start; i < end; i++)
    {
        s += arr[i];
    }
    pthread_mutex_lock(&sum_mutex);
    total_sum += s;
    pthread_mutex_unlock(&sum_mutex);
    return NULL;
}

int main()
{
    Create_random_array();
    pthread_mutex_init(&sum_mutex, NULL);

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        int* thread_id = malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&threads[i], NULL, sum, (void*)thread_id);
        //Lý do sử dụng câu lệnh trên là do sau khi build không lỗi những chạy app thì bị lỗi Segmentation fault 
        //Chương trình cố gắng truy cập bộ nhớ không hợp lệ
        //Như truy cập một địa chỉ ngoài phạm vi mảng hoặc sử dụng con trỏ chưa được khởi tạo đúng cách.
        //Như vậy ta cần cấp bộ phát bộ nhớ động bằng malloc() 
        //pthread_create(&threads[i], NULL, sum, (void*)&thread_ids[i])
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Total sum of the array: %lld\n", total_sum);
    pthread_mutex_destroy(&sum_mutex);

    return 0;
}