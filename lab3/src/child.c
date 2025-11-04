#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>


#define SHM_SIZE 4096

const char SHM_NAME[] = "example_sh_memory";
const char SEM_NAME[] = "example_semaphore";


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        const char message[] = "Error: No filename provided\n";
        write(STDERR_FILENO, message, sizeof(message));
        _exit(EXIT_FAILURE);
    }

    int shared_mem = shm_open(SHM_NAME, O_RDWR, 0600);
    if (shared_mem == -1)
    {
        const char message[] = "Error: Unable to open shared_memory\n";
        write(STDERR_FILENO, message, sizeof(message));
        _exit(EXIT_FAILURE);
    }

    char* shared_mem_buffer = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_mem, 0);
    if (shared_mem_buffer == MAP_FAILED)
    {
        const char message[] = "Error: Unable to map shared_memory\n";
        write(STDERR_FILENO, message, sizeof(message));
        _exit(EXIT_FAILURE);
    }

    sem_t* semaphore = sem_open(SEM_NAME, O_RDWR);
    if (semaphore == SEM_FAILED)
    {
        const char message[] = "Error: Unable to open semaphore\n";
        write(STDERR_FILENO, message, sizeof(message));
        _exit(EXIT_FAILURE);
    }

    int file = open(argv[1], O_RDONLY);
    if (file == -1)
    {
        const char message[] = "Error: Unable to open file";
        write(STDERR_FILENO, message, sizeof(message));
        _exit(EXIT_FAILURE);
    }

    int bytes;
    const int BUFFER_SIZE = 4096 - sizeof(int);
    char buffer[BUFFER_SIZE];

    int running = 1;
    while ((bytes = read(file, buffer, BUFFER_SIZE - 1)) > 0)
    {
        buffer[bytes] = 0;
        int numbers[128];
        int counter = 0;
        char* ptr = buffer;
        
        while(*ptr != 0)
        {
            if (*ptr == '\n')
            {
                ptr++;

                if (counter >= 2)
                {
                    int result = numbers[0];
                    
                    int valid = 1;
                    for (int i = 1; i < counter; ++i)
                    {
                        if (numbers[i] == 0)
                        {
                            const char err_msg[] = "error: Division by zero\n";
                            write(STDERR_FILENO, err_msg, sizeof(err_msg));
                            valid = 0;
                            break;
                        }

                        result /= numbers[i];
                    }
                    
                    if (valid)
                    {
                        char output[50];
                        int size = snprintf(output, sizeof(output), "%d\n", result);

                        usleep(10000);

                        sem_wait(semaphore);

                        int* length = (int*)shared_mem_buffer;
                        char* text = shared_mem_buffer + sizeof(int);
                        *length = size;
                        memcpy(text, output, size);

                        sem_post(semaphore);
                    }
                }
                else if (counter > 0)
                {
                    const char message[] = "error: Not enough arguments passed\n";
                    write(STDERR_FILENO, message, sizeof(message));
                }
                
                counter = 0;
            }

            if (isdigit(*ptr))
            {
                if (counter < 128)
                {
                    if (sscanf(ptr, "%d", &numbers[counter]) > 0)
                    {
                        counter++;
                    }
                }
                
                while (isdigit((unsigned char)*ptr))
                {
                    ptr++;
                }
            }
            else 
            {
                ptr++;
            }

        }

    }

    sem_wait(semaphore);
    
    int* length = (int*)shared_mem_buffer;
    *length = INT_MAX;

    sem_post(semaphore);

    close(file);
    sem_close(semaphore);
    munmap(shared_mem_buffer, SHM_SIZE);
    close(shared_mem);

    return 0;
}