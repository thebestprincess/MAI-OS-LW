#include "libs.h"
#include <math.h>
#include <stddef.h>


float sin_integral(float a, float b, float e)
{
    float sum = 0.0;

    for (float x = a; x < b; x += e)
    {
        sum += sinf(x) * e;
    }

    return sum;
}


int* sort(int* array, size_t size)
{
    if (size < 2) return array;

    for (size_t i = 0; i < size - 1; ++i)
    {
        for (size_t j = 0; j < size - i - 1; ++j)
        {
            if (array[j] > array[j + 1])
            {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp; 
            }
        }
    }

    return array;
}
