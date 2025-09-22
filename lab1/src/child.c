#include <unistd.h>
#include <stdlib.h>


int string_to_int(const char *str) 
{
    int result = 0;
    int sign = 1;
    int i = 0;
    
    if (str[0] == '-') 
    {
        sign = -1;
        i = 1;
    }
    
    while (str[i] != '\0') 
    {
        if (str[i] >= '0' && str[i] <= '9') 
        {
            result = result * 10 + (int)(str[i] - '0');
        }
        i++;
    }
    
    return result * sign;
}


void int_to_string(int num, char *str) 
{
    int i = 0;
    int is_negative = 0;
    
    if (num < 0) 
    {
        is_negative = 1;
        num = -num;
    }
    
    do 
    {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);
    
    if (is_negative) 
    {
        str[i++] = '-';
    }
    
    str[i] = '\0';
    
    int start = 0;
    int end = i - 1;
    while (start < end) 
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}


int main(int argc, char** argv)
{
    char buff[1024];
    ssize_t bytes;

    while ((bytes = read(STDIN_FILENO, buff, sizeof(buff) - 1)) > 0)
    {
        int numbers[99];    // Numbers storage
        int count = 0;      // Counter of the quantity of numbers 
        int ptr = 0;        // Ptr to the first character of the number

        for (int i = 0; i < bytes; ++i)
        {
            if (buff[i] == ' ' || buff[i] == '\n' || buff[i] == '\t')
            {
                if (i > ptr)
                {
                    char number[20];
                    int number_length = i - ptr;
                    for (int j = 0; j < number_length; ++j)
                    {
                        number[j] = buff[ptr + j];
                    }
                    number[number_length] = '\0';
                
                    numbers[count++] = string_to_int(number);
                }
                ptr = i + 1;
                
                if (buff[i] == '\n' && count > 0)
                {
                    if (count < 2)
                    {
                        const char err_msg[] = "error: Not enough arguments passed\n";
                        write(STDERR_FILENO, err_msg, sizeof(err_msg));
                    }
                    else
                    {
                        int result = numbers[0];
                        
                        for (int j = 1; j < count; j++) 
                        {
                            if (numbers[j] == 0) 
                            {
                                const char err_msg[] = "error: Division by zero\n";
                                write(STDERR_FILENO, err_msg, sizeof(err_msg));
                                exit(EXIT_FAILURE);
                            }
                            result /= numbers[j];
                        }
                        char result_str[20];
                        
                        char output[50];
                        int pos = 0;
                        
                        int_to_string(result, result_str);
                        for (int k = 0; result_str[k] != '\0'; k++) 
                        {
                            output[pos++] = result_str[k];
                        }
                        output[pos++] = '\n';
                        
                        write(STDOUT_FILENO, output, pos);
                    }
                    
                    count = 0;
                }
            }
        }
    }

}