#include "libs.h"
#include <math.h>


float sin_integral(float a, float b, float e)
{
    float sum = 0.0;

    for (float x = a; x < b; x += e)
    {
        sum += 0.5 * (sinf(x) + sinf(x + e)) * e;
    }

    return sum;
}


int partition(int* array, int start, int end)
{
    int pivot = array[end];
    int i = start - 1;

    for (int j = start; j <= end - 1; ++j)
    {
        if (array[j] < pivot)
        {
            i++;
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
    i++;
    int temp = array[i];
    array[i] = array[end];
    array[end] = temp;

    return i;
}


void quick_sort(int* array, int start, int end)
{
    if (end <= start) return;
    
    int pivot = partition(array, start, end);
    quick_sort(array, start, pivot - 1);
    quick_sort(array, pivot + 1, end);
}


int* sort(int* array, size_t size)
{
    if (size > 0) quick_sort(array, 0, (int)size - 1);
    return array;
}