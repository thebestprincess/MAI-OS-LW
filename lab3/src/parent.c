#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define SHM_SIZE 4096

const char SHM_NAME[] = "example_sh_memory";
const char SEM_NAME[] = "example_semaphore";

int main(int argc, char** argv)
{
    int shared_mem = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shared_mem == -1 && errno != ENOENT)
    {
        const char message[] = "Error: Unable to open shared_mem\n";
        write(STDERR_FILENO, message, sizeof(message)); 
        exit(EXIT_FAILURE);
    }

    shared_mem = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (shared_mem == -1)
    {
        const char message[] = "Error: Unable to create shared_mem\n";
        write(STDERR_FILENO, message, sizeof(message));
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shared_mem, SHM_SIZE) != 0)
    {
        const char message[] = "Error: Unable to resize shared_mem\n";
        write(STDERR_FILENO, message, sizeof(message));
        exit(EXIT_FAILURE);
    }

    char* const shared_mem_buffer = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_mem, 0);
    if (shared_mem_buffer == MAP_FAILED)
    {
        const char message[] = "Error: Unable to map shared_mem\n";
        write(STDERR_FILENO, message, sizeof(message));
        exit(EXIT_FAILURE);
    }


    sem_t* semaphore = sem_open(SEM_NAME, O_RDWR | O_CREAT, 0666, 1); 
    if (semaphore == SEM_FAILED)
    {
        const char message[] = "Error: Unable to open semaphore\n";
        write(STDERR_FILENO, message, sizeof(message));
        exit(EXIT_FAILURE);
    }

    char file_path[128];
    {
        const char message[] = "Input filename: ";
        write(STDOUT_FILENO, message, sizeof(message));

        int result = read(STDIN_FILENO, file_path, sizeof(file_path) - 1);
        if (result <= 0)
        {
            const char error_message[] = "Error: Unable to read from standard input\n";
            write(STDERR_FILENO, error_message, sizeof(error_message));
            exit(EXIT_FAILURE);
        }   

        file_path[result - 1] = 0;
    }


    pid_t child = fork();

    if (child == 0)
    {
        execl("./child", "client", file_path, NULL);

        const char message[] = "Error: Unable to execv\n";
        write(STDERR_FILENO, message, sizeof(message));
        exit(EXIT_FAILURE);
    }
    else if (child == -1)
    {
        const char message[] = "Error: Unable to fork";
        write(STDERR_FILENO, message, sizeof(message));
        exit(EXIT_FAILURE);
    }

    
    int running = 1;
    while(running)
    {
        sem_wait(semaphore);

        int* length = (int*)shared_mem_buffer;
        char* data = shared_mem_buffer + sizeof(int);

        if (*length == INT_MAX)
        {
            running = 0;
        }
        else if (*length > 0)
        {
            write(STDOUT_FILENO, data, *length);
            *length = 0;
        }
        
        sem_post(semaphore);
    }


    waitpid(child, NULL, 0);

    sem_unlink(SEM_NAME);
    sem_close(semaphore);

    munmap((void*)shared_mem_buffer, SHM_SIZE);

    shm_unlink(SHM_NAME);
    close(shared_mem);

    return 0;
}