#include <unistd.h>
#include <stdio.h>      // snprintf
#include <stdlib.h>
#include <string.h>

#include "libs.h"


#define BUFFER_SIZE 4096


void command_1()
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

void command_2()
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
    {
        const char* message = "Program 1 (Static).\nCommands: 1 a b e | 2 Size Array... | Ctrl + D to exit\n> ";
        write(STDOUT_FILENO, message, strlen(message));
    }

    int bytes_read = 0;
    char buffer[BUFFER_SIZE];

    while((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1)) > 0)
    {
        buffer[bytes_read] = 0;

        char* token = strtok(buffer, " \t\n");
        if (!token) continue;

        int cmd = atoi(token);
        switch (cmd)
        {
            case 1:
            {
                command_1();
                break;
            }
            case 2:
            {
                command_2();
                break;
            }
        }

        write(STDOUT_FILENO, "> ", 2);
    }

    return 0;
}