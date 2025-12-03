#include <stddef.h>
#include <unistd.h>
#include <stdio.h>      // snprintf
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>


#define BUFFER_SIZE 4096


typedef float (*sin_integral_func)(float, float, float);
typedef int* (*sort_func)(int*, size_t);


enum ErrorCode 
{
    OK = 0,
    ER_DLOPEN = -1,
};

enum CurrentLib
{
    FIRST = 0,
    SECOND = 1,
};


enum ErrorCode command_0(const char** LIB_NAMES, void** library, int* current_lib,
                        sin_integral_func* sin_integral, sort_func* sort)
{
    dlclose(*library);
    switch (*current_lib)
    {
        case FIRST: *current_lib = SECOND; break;
        case SECOND: *current_lib = FIRST; break;
    }

    char buffer[BUFFER_SIZE];

    *library = dlopen(LIB_NAMES[*current_lib], RTLD_LAZY);
    if (!(*library))
    {
        int len = snprintf(buffer, BUFFER_SIZE, "Error switching: %s\n", dlerror());
        write(STDERR_FILENO, buffer, len);
        return ER_DLOPEN;
    }


    *sin_integral = dlsym(*library, "sin_integral");
    if (!sin_integral)
    {
        const char msg[] = "warning: failed to find sin_integral function implementation\n";
        write(STDERR_FILENO, msg, sizeof(msg)); 
    }

    *sort = dlsym(*library, "sort");
    if (!sort)
    {
        const char msg[] = "warning: failed to find sort function implementation\n";
        write(STDERR_FILENO, msg, sizeof(msg)); 
    }

    {
        int len = snprintf(buffer, BUFFER_SIZE, "Switched to library: %s\n", LIB_NAMES[*current_lib]);
        write(STDOUT_FILENO, buffer, len);
    }

    return OK;
}


void command_1(sin_integral_func sin_integral)
{
    char* arg1 = strtok(NULL, " \t\n");
    char* arg2 = strtok(NULL, " \t\n");
    char* arg3 = strtok(NULL, " \t\n");
    
    int len = 0;
    char buffer[BUFFER_SIZE];

    if (arg1 && arg2 && arg3)
    {
        float res = sin_integral(atof(arg1), atof(arg2), atof(arg3));
        len = snprintf(buffer, BUFFER_SIZE, "Integral result: %.5f\n", res);

        write(STDOUT_FILENO, buffer, len);
    }
}

void command_2(sort_func sort)
{
    char* size_str = strtok(NULL, " \t\n");
    if (!size_str) return;

    int size = atoi(size_str);
    int* array = (int*)malloc(size * sizeof(int));
    if (!array)
    {
        const char* message = "Unable to malloc memory for int* array\n";
        write(STDERR_FILENO, message, strlen(message));
        return;
    }

    for (int i = 0; i < size; ++i)
    {
        char* value = strtok(NULL, " \t\n");
        if (value) array[i] = atoi(value);
        else array[i] = 0;
    }

    sort(array, size);

    {
        const char* message = "Sorted array: ";
        write(STDOUT_FILENO, message, strlen(message));
    }

    char buffer[BUFFER_SIZE];
    for (int i = 0; i < size; ++i)
    {
        int len = snprintf(buffer, BUFFER_SIZE, "%d ", array[i]);
        write(STDOUT_FILENO, buffer, len);
    }

    write(STDOUT_FILENO, "\n", 1);
    free(array);
}


int main()
{
    const char* LIB_NAMES[] = {"./libd1.so", "./libd2.so"};
    int current_lib = FIRST;

    sin_integral_func sin_integral = NULL;
    sort_func sort = NULL;

    char buffer[BUFFER_SIZE];

    void* library = dlopen(LIB_NAMES[current_lib], RTLD_LAZY);
    if (!library)
    {
        int len = snprintf(buffer, BUFFER_SIZE, "Error loading library: %s\n", dlerror());
        write(STDERR_FILENO, buffer, len);
        return ER_DLOPEN;
    }

    sin_integral = dlsym(library, "sin_integral");
    if (!sin_integral)
    {
        const char msg[] = "warning: failed to find sin_integral function implementation\n";
        write(STDERR_FILENO, msg, sizeof(msg)); 
    }

    sort = dlsym(library, "sort");
    if (!sort)
    {
        const char msg[] = "warning: failed to find sin_integral function implementation\n";
        write(STDERR_FILENO, msg, sizeof(msg)); 
    }

    {
        const char *msg = "Program 2 (Dynamic).\nCommands: 0 (Switch) | 1 A B E | 2 Size Arr...\n> ";
        write(STDOUT_FILENO, msg, strlen(msg));
    }

    int bytes_read;

    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1)) > 0)
    {
        buffer[bytes_read] = '\0';

        char *token = strtok(buffer, " \t\n");
        if (!token) continue;

        int cmd = atoi(token);
        switch (cmd)
        {
            case 0:
            {
                int result = command_0(LIB_NAMES, &library, &current_lib, &sin_integral, &sort);
                if (result != OK) return result;
                break;
            }
            case 1:
            {
                command_1(sin_integral);
                break;
            }
            case 2:
            {
                command_2(sort);
                break;
            }
        }

        write(STDOUT_FILENO, "> ", 2);
    }

    if (library) dlclose(library);
    return OK;
}
