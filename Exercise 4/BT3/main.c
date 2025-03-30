#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

// #define NUM_THREAD 3
#define NUM_REPETITION 10

int data;

pthread_mutex_t data_mutex;
pthread_cond_t data_cond;

void* Producer(void* arg)
{
    for (int i = 0; i < NUM_REPETITION; i++)
    {
        int value = rand() % 10 + 1;
        pthread_mutex_lock(&data_mutex); 
        data = value;
        pthread_cond_signal(&data_cond);
        pthread_mutex_unlock(&data_mutex);
        sleep(1);
    }
    return NULL;
}
void* Consumer(void* arg)
{
    for (int i = 0; i < NUM_REPETITION; i++)
    {
        pthread_mutex_lock(&data_mutex); 
        while (data == 0)
        {
            pthread_cond_wait(&data_cond, &data_mutex);
        }
        printf("Consumer: Consumed data = %d\n", data);
        data = 0;
        pthread_mutex_unlock(&data_mutex);
        sleep(1);
    }
    return NULL;
}
int main()
{
    pthread_t producer_thread, consumer_thread;
    pthread_mutex_init(&data_mutex, NULL);
    pthread_cond_init(&data_cond, NULL);

    pthread_create(&producer_thread, NULL, Producer, NULL);
    pthread_create(&consumer_thread, NULL, Consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_mutex_destroy(&data_mutex);
    pthread_cond_destroy(&data_cond);

    return 0;
}