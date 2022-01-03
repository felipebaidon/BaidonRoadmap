/******************************************************************************
 * Copyright (C) 2017 by Alex Fosdick - University of Colorado
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are 
 * permitted to  modify this and use it to learn about the field of embedded
 * software. Alex Fosdick and the University of Colorado are not liable for any
 * misuse of this material. 
 *
 *****************************************************************************/
/**
 * @file stats.h 
 * @brief File used to perform statistics on a given array 
 *
 *  This file contains function prototypes and global variables used to perform
 *  statistics on a given array 
 *
 * @author Felipe Baidon
 * @date 22 Dec 2021
 *
 */
#ifndef __STATS_H__
#define __STATS_H__

/* Add Your Declarations and Function Comments here */ 

/**
 * @brief This function prints the stadistical data of a received array
 *
 * @param data An array that will be analyzed
 * @param arraySize The size of the array
 *
 */

void print_statistics(unsigned char *data, int arraySize);

/**
 * @brief This function prints an  array
 *
 * @param data The array to be printed 
 * @param arraySize The size of the array to be printed 
 *
 */

void print_array(const unsigned char *data, int arraySize);

/**
 * @brief This function calculates the median of an  array
 *
 * @param data The array to calculate the median from 
 * @param arraySize The size of the array to calculate the medain from 
 *
 * @return the median of the array
 */

unsigned char find_median(unsigned char *data, int arraySize);

/**
 * @brief This function calculates the mean of an array
 *
 * @param data The array whose mean would be calculated 
 * @param arraySize The size of the arraw whose mean would be calculated 
 *
 * @return the mean of an array
 */

unsigned char find_mean(const unsigned char *data, int arraySize);

/**
 * @brief This function finds the maximum value of an array
 *
 * @param data The array to whose maximum would be obtained 
 * @param arraySize The size of the array whose maximum would be obtained 
 *
 * @return The maximum value of the array
 * 
 */

unsigned char find_maximum(const unsigned char *data, int arraySize);

/**
 * @brief This function finds the minimum value of an array
 *
 * @param data The array to whose minimum would be obtained 
 * @param arraySize The size of the array whose minimum would be obtained 
 *
 * @return The minimum value of the array
 * 
 */
unsigned char find_minimum(const unsigned char *data, int arraySize);

/**
 * @brief This function sorts the array
 *
 * @param data The array to be sorted 
 * @param arraySize The size of the array to be sorted 
 *
 * @return A sorted Array 
 * 
 */
unsigned char* sort_array(unsigned char *data, int arraySize);

    


#endif /* __STATS_H__ */
