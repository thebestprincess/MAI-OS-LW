#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../include/validator.h"


typedef struct
{
    int thread_id;
    int PTH_NUMBER;
    int ARR_NUMBER;
    int ARRAY_SIZE;
    int** arrays;
} ThreadInfo;


void* computing(void* arg)
{
    ThreadInfo thread_info = *((ThreadInfo*)arg);

    int size = thread_info.ARRAY_SIZE / thread_info.PTH_NUMBER;
    int index = thread_info.thread_id * size;
    int end = (thread_info.thread_id == thread_info.PTH_NUMBER - 1) ? thread_info.ARRAY_SIZE : index + size;
    long long* sum = malloc(sizeof(long long));
    *sum = 0;

    char buffer[128];
    if (sprintf(buffer, "Thread (id: %d) is working...\n", thread_info.thread_id))
    {
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }

    for (int i = 0; i < thread_info.ARR_NUMBER; ++i)
    {
        for (int j = index; j < end; ++j)
        {
            *sum += thread_info.arrays[i][j];
        }
    }

    return (void*)sum;
}


int main(int argc, char** argv)
{
    int number = args_processing(argc, argv);

    if (number != 0)
    {
        char buffer[64];
        char* message = "Error: unable to get number of threads from CL\n";
        sprintf(buffer, "Use: ./%s <PTH_NUMBER> <ARR_NUMBER> <ARRAY_SIZE>\n", argv[0]);
        write(STDOUT_FILENO, message, strlen(message));
        write(STDOUT_FILENO, buffer, strlen(buffer));
        exit(EXIT_FAILURE);
    }


    // Checking the value of constants
    const int PTH_NUMBER = atoi(argv[1]);
    const int ARR_NUMBER = atoi(argv[2]);
    const int ARRAY_SIZE = atoi(argv[3]);

    if (ARRAY_SIZE <= 0 || PTH_NUMBER <= 0 || ARR_NUMBER <= 0)
    {
        char* message = "Error: All args must be positive.\n";
        write(STDIN_FILENO, message, strlen(message));
        exit(EXIT_FAILURE);
    }


    
    int** arrays = (int**)malloc(ARR_NUMBER * sizeof(int*));

    if (arrays == NULL)
    {
        char* message = "Error: unable to allocate memory for arrays\n";
        write(STDOUT_FILENO, message, strlen(message));
        exit(EXIT_FAILURE);
    }

    // Fill arrays
    char temp_buffer[ARRAY_SIZE];
    long long inter_sum = 0;
    srand(time(NULL));

    for (int i = 0; i < ARR_NUMBER; ++i)
    {
        arrays[i] = (int*)malloc(ARRAY_SIZE * sizeof(int));

        if (arrays[i] == NULL)
        {
            char* message = "Error: unable to allocate memory for numbers of arrays\n";
            write(STDOUT_FILENO, message, strlen(message));
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < ARRAY_SIZE; ++j)
        {
            arrays[i][j] = rand() % 100 + 1;
            inter_sum += arrays[i][j];
        }
    }


    // Create threads and ThreadInfo for each thread
    pthread_t thread[PTH_NUMBER];
    ThreadInfo* threads = malloc(PTH_NUMBER * sizeof(ThreadInfo));

    for (int i = 0; i < PTH_NUMBER; ++i)
    {
        threads[i] = (ThreadInfo)
        {
            .thread_id = i,
            .PTH_NUMBER = PTH_NUMBER,
            .ARR_NUMBER = ARR_NUMBER,
            .ARRAY_SIZE = ARRAY_SIZE,
            .arrays = arrays,
        };

        if (pthread_create(thread + i, NULL, computing, (void*)(threads + i)) != 0)
        {
            char* message = "Error: unable to create pthread\n";
            write(STDOUT_FILENO, message, strlen(message));
            exit(EXIT_FAILURE);  
        }
    }


    // Receive intermediate sum from threads and calculate globalSum 
    long long* result;
    long long globalSum = 0;

    for (int i = 0; i < PTH_NUMBER; ++i)
    {
        if (pthread_join(thread[i], (void**)&result) != 0)
        {
            char* message = "Error: unable to join pthread\n";
            write(STDOUT_FILENO, message, strlen(message));
            exit(EXIT_FAILURE);  
        }
        globalSum += *result;
        free((void*)result);    // Memory was allocated in void* computing(void* args)
    }

    // Output globalSum
    if (sprintf(temp_buffer, "Global sum is: %lld == %lld :inter_sum\n", globalSum, inter_sum) >= 0)
    {
        write(STDOUT_FILENO, temp_buffer, strlen(temp_buffer));
    }
    

    // Free allocated memory
    for (int i = 0; i < ARR_NUMBER; ++i)
    {
        free((void*)arrays[i]);
    }

    free((void*)arrays);
    free((void*) threads);

    return 0;
}