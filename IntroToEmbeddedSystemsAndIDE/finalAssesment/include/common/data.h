#include <stdint.h>
/*
*
*@brief This function is used to convert an integer to a string of numbers
*
* This function receives the integer to be converted to a string, which will
* be stored in a pointer passed as a parameter the converted string depends
* on the base passed as a parameter. The function handles negative numbers
*
*@param data The integer to be converted to string
*@param ptr The pointer to store the final string
*@param base The desired base to which the nuber will be converted to, 2 to 16
*
*@return The length of the converted string 
*
*/
uint8_t my_itoa(int32_t data, uint8_t *ptr, uint32_t base);


/*
*
*@brief This fuction is used to convert a string of numbers to an integer
*
*This function receives a pointer containg a string of numbers and its digits
*to be converted to an integer depending on the base, the function handles
*negative numbes and bases from 2 to 16
*
*@param ptr The start of memory location containg the string of numbers
*@param digits The number of digits contained in the string of numbers
*@param base The base used in the string of numbers 
*
*@return The string of numbers converted to integer
*
*/
int32_t my_atoi(uint8_t* ptr, uint8_t digits, uint32_t base);
