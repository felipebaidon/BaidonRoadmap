/******************************************************************************
 * Copyright (C) 2017 by Alex Fosdick - University of Colorado
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are 
 * permitted to modify this and use it to learn about the field of embedded
 * software. Alex Fosdick and the University of Colorado are not liable for any
 * misuse of this material. 
 *
 *****************************************************************************/
/**
 * @file stats.c 
 * @brief This file contains the function definitions to perform stadistical     *         analysis on a given array 
 *
 * <Add Extended Description Here>
 *
 * @author Felipe Baidon
 * @date 21 Dec 2021
 *
 */



#include <stdio.h>
#include "stats.h"

/* Size of the Data Set */
#define SIZE (40)

void main() {

  unsigned char test[SIZE] = { 34, 201, 190, 154,   8, 194,   2,   6,
                              114, 88,   45,  76, 123,  87,  25,  23,
                              200, 122, 150, 90,   92,  87, 177, 244,
                              201,   6,  12,  60,   8,   2,   5,  67,
                                7,  87, 250, 230,  99,   3, 100,  90};

  /* Other Variable Declarations Go Here */
  /* Statistics and Printing Functions Go Here */
  
  printf("Printing Statistics\r\n");
  print_statistics(&test[0], SIZE);
  
  printf("Printing Array\r\n");
  print_array(&test[0], SIZE);

  printf("Sorting array\r\n");
  sort_array(&test[0], SIZE);

  printf("Printing Array\r\n");
  print_array(&test[0], SIZE);
}

/* Add other Implementation File Code Here */
unsigned char find_mean(const unsigned char *data, int arraySize)
{
    unsigned char sum = 0;
    int i = 0;

    for(; i < arraySize; i++)
    {
        sum += data[i];
    }
    
    return sum/i;
}

unsigned char find_maximum(const unsigned char *data, int arraySize)
{

    unsigned char maximumValue = *data;

    for(int i = 1; i < arraySize; i++)
    {
        if( maximumValue < data[i] ) 
        {
            maximumValue = data[i];
        }
    }

    return maximumValue;
}

unsigned char find_minimum(const unsigned char *data, int arraySize)
{

    unsigned char minimumValue = *data;

    for(int i = 1; i < arraySize; i++)
    {
        if( minimumValue > data[i] )
        {
            minimumValue = data[i];
        }
    }

    return minimumValue;
}

unsigned char* sort_array(unsigned char *data, int arraySize)
{
    unsigned char temp = 0;
    for(int i= 0; i<arraySize ; i++)
    {
        for(int j = i +1; j < arraySize; j++)
        {
            if(data[i] < data[j] )
            {
                temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
     }

return data;
}

unsigned char find_median(unsigned char *data, int arraySize)
{
    unsigned char* sortedArray = sort_array(&data[0], arraySize);
    unsigned char median = 0;
    
    if(arraySize % 2 != 0)
    {
        median = sortedArray[(int)(arraySize/2)];
    }
    else 
    {
        median= sortedArray[arraySize/2] + sortedArray[arraySize/2 + 1];
    }

    return median;
}

void print_statistics( unsigned char *data, int arraySize)
{
    unsigned char minimum = find_minimum(data,arraySize);
    unsigned char maximum = find_maximum(data, arraySize);
    unsigned char mean = find_mean(data, arraySize);
    unsigned char median = find_median(data, arraySize);

    printf( " Minimum value of the array: %d\r\n Maximum value of the array: %d\r\n Mean value of the array: %d\r\n Median value of the array: %d\r\n", minimum, maximum, mean, median);

}

void print_array(const unsigned char *data, int arraySize)
{
    for(int i= 0; i < arraySize; i++)
    {
        printf("Element %d of Array: %d\r\n", i, data[i]);
    }
}
