#include "../include/validator.h"

#include <ctype.h>

// Returns 0 if the number is natural, otherwise 1 
int validate_int(char* string)
{
    while (*string != 0)
    {
        if (!isdigit(*string))
        {
            return 1;
        }
        
        ++string;
    }

    return 0;
}


int args_processing(int argc, char **argv)
{
    if (argc != 4)
    {
        // Args wasn't passed 
        return 1;
    }

    for (int i = 1; i < 4; ++i)   
    {        
        if (validate_int(argv[i]) != 0)
        {
            // Invalid argument
            return 2;
        }
    }

    return 0;
}