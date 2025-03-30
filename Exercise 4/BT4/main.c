#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define Array_size 100

int arr[Array_size];
int even_count, odd_count;

void Create_random_array()
{
    for (int i = 0; i < Array_size; i++)
    {
        arr[i] = rand() % 100 + 1;
    }
}
void* Even_number(void* arg)
{
    even_count = 0;
    for (int i = 0; i < Array_size; i++)
    {
        if (arr[i] % 2==0)  
        {
            even_count++;
        }
    }
    return NULL;
}
void* Odd_number(void* arg)
{
    odd_count = 0;
    for (int i = 0; i < Array_size; i++)
    {
        if (arr[i] % 2!=0)  
        {
            odd_count++;
        }
    }
    return NULL;
}
int main()
{
    Create_random_array();
    pthread_t even_thread, odd_thread;

    pthread_create(&even_thread, NULL, Even_number, NULL);
    pthread_create(&odd_thread, NULL, Odd_number, NULL);

    pthread_join(even_thread, NULL);
    pthread_join(odd_thread, NULL);

    printf("Total even numbers: %d\n", even_count);
    printf("Total odd numbers: %d\n", odd_count);

    return 0;
}