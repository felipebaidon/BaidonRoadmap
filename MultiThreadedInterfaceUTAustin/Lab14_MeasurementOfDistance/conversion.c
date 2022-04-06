// conversion.c
// Runs on LM4F120 or TM4C123, 
// for edX lab 14
// This module manages the conversion of data to string and formating
// Felipe Baidon
// March 19, 2022

#include "conversion.h"

//-----------------------ConvertDistance-----------------------
// Converts a 32-bit distance into an ASCII string
// Input: 32-bit number to be converted (resolution 0.001cm)
// Output: store the conversion in global variable String[10]
// Fixed format 1 digit, point, 3 digits, space, units, null termination
// Examples
//    4 to "0.004 cm"  
//   31 to "0.031 cm" 
//  102 to "0.102 cm" 
// 2210 to "2.210 cm"
//10000 to "*.*** cm"  any value larger than 9999 converted to "*.*** cm"
void ConvertDistance(unsigned long n){
	int divisor=1000;
	int i= 0;
	
	if( n <= 9999)
	{
		
		String[i++]= n/divisor + '0';
		n= n%divisor;
		divisor/=10;
		
		String[i++]='.';
		
		while(i<5)
		{
			String[i++] = n/divisor+ '0';
			n%= divisor;
			divisor/=10;
		}
		
	}
	else
	{
		String[i++]= '*';
		String[i++]='.';
		while(i<5)
		{
			String[i++] = '*';
		}		
	}
	
		String[i++] = ' ';
		String[i++] = 'c';
		String[i++] = 'm';
	  String[i++] = '\r';
	  String[i++] = '\n';
		String[i] = '\0';
}

//-----------------------Convert-----------------------
// Convert a 12-bit binary ADC sample into a 32-bit unsigned
// fixed-point distance (resolution 0.001 cm).  Calibration
// data is gathered using known distances and reading the
// ADC value measured on PE1.  
// Overflow and dropout should be considered 
// Input: sample  12-bit ADC sample
// Output: 32-bit distance (resolution 0.001cm)
unsigned long Convert(unsigned long sample){
	const unsigned long A = 431;
	const unsigned long B = 762;
	unsigned int distance = 0;
	
	distance = ((A*sample) >> 10) + B;
	
  return distance;
}

