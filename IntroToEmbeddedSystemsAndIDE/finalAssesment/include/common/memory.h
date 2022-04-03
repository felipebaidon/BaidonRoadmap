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
 * @file memory.h
 * @brief Abstraction of memory read and write operations
 *
 * This header file provides an abstraction of reading and
 * writing to memory via function calls. 
 *
 * @author Alex Fosdick
 * @date April 1 2017
 *
 */
#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>
#include <stddef.h>
/**
 * @brief Sets a value of a data array 
 *
 * Given a pointer to a char data set, this will set a provided
 * index into that data set to the value provided.
 *
 * @param ptr Pointer to data array
 * @param index Index into pointer array to set value
 * @param value value to write the the locaiton
 *
 * @return void.
 */
void set_value(char * ptr, unsigned int index, char value);

/**
 * @brief Clear a value of a data array 
 *
 * Given a pointer to a char data set, this will clear a provided
 * index into that data set to the value zero.
 *
 * @param ptr Pointer to data array
 * @param index Index into pointer array to set value
 *
 * @return void.
 */
void clear_value(char * ptr, unsigned int index);

/**
 * @brief Returns a value of a data array 
 *
 * Given a pointer to a char data set, this will read the provided
 * index into that data set and return the value.
 *
 * @param ptr Pointer to data array
 * @param index Index into pointer array to set value
 *
 * @return Value to be read.
 */
char get_value(char * ptr, unsigned int index);

/**
 * @brief Sets data array elements to a value
 *
 * Given a pointer to a char data set, this will set a number of elements
 * from a provided data array to the given value. The length is determined
 * by the provided size parameter.
 *
 * @param ptr Pointer to data array
 * @param value value to write the the locaiton
 * @param size Number of elements to set to value
 *
 * @return void.
 */
void set_all(char * ptr, char value, unsigned int size);

/**
 * @brief Clears elements in a data array
 *
 * Given a pointer to a char data set, this will set a clear a number
 * of elements given the size provided. Clear means to set to zero. 
 *
 * @param ptr Pointer to data array
 * @param size Number of elements to set to zero
 *
 * @return void.
 */
void clear_all(char * ptr, unsigned int size);

/*
*@brief This function takes two byte pointers and a length of bytes 
* 	to move from the source location to destination
*
*The function handles memory overlap and uses pointer arithmetic to handle
* memory manipulation 
*
*@param src The source of the memory allocation
*@param dst The destination of the memory allocation
*
*@return A pointer to the destination
*/
uint8_t* my_memmove(uint8_t* src, uint8_t* dst, size_t length);

/*
*@brief This function takes two byte pointers to copy a source location
*	to destination
*
*The function does not handle memory overlap, the behaviour is undefined when
* that happens

*@param src The source of the memory to copy
*@param dst The destination of the memory
*
*@return A pointer to the destination of copy
*/
uint8_t* my_memcopy(uint8_t* src, uint8_t* dst, size_t length);

/*
*@brief This function copies the specified value to the length of the 
*	memory specified
*
*The operations of this function use pointer arithmetic 
*
*@param src The source of the allocated memory 
*@param length The size of the memory that it is being passed
*@param value The value that will be copied throughout the memory
*
*@return A pointer to the source
*/
uint8_t* my_memset(uint8_t* src, size_t length, uint8_t value);

/*
*
*@brief This function fills with zeros the specified memory
*
*This function fill with zeros from the base address of the memory
* to the specified length
*
*@param src The pointer indicating the base addres of the allocated memory
*@param length The size of the memory that will be filled with zeros
*
*@return A pointer to the source
*/
uint8_t* my_memzero(uint8_t* src, size_t length);

/*
*
*@brief This function reverses the order of the content on the allocated memory
*
*This function takes a pointer to the source and the length of the pool of data
*to return a reversed order within that memory allocation
*
*@param src A pointer to the beggining of the memory allocation
*@param length The length of the pool of data
*
*@return A pointer to the source
*/
uint8_t* my_reverse(uint8_t* src, size_t length);

/*
*
*@brief This function reserves a number of words in dynamic memory
*
* This function takes the length as a parameter to reserve dynamic memory
*
*@param length The size of the memory to allocate dynamically
*
*@return A pointer to the pool of memory if succesfull, NULL otherwise
*/
int32_t* reserve_words(size_t length);

/*
*
*@brief This function frees a pool of previously dinamically-allocated memory
*
*This function takes as a parameter a pointer to a pool of memory that was
* previously allocated
*
*@param src The pointer to the memory that was previously allocated
*
*@return none
*
*/
void free_words(int32_t* src);
 
#endif /* __MEMORY_H__ */
