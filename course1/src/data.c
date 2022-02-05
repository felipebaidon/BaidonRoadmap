#include <stdint.h>
#include <stdbool.h>
#include "platform.h"
#include <math.h>

uint8_t my_itoa(int32_t data, uint8_t *ptr, uint32_t base)
{
  uint8_t numberDigits = 0;
  uint8_t currentIndex = 0;
  int baseVal = 0;
  int numVal = 0;
  char value;

/*Check for 0 input value */
  if(data== 0)
  {
    *ptr = '0';
    ptr++;
    currentIndex++;
    *ptr = '\0';
    return numberDigits=1;
  }
/* Check negative values */
  if(data < 0)
  {
    numberDigits++;
    *ptr= '-';
    ptr++;
    currentIndex++;
    data*=-1;
  }

  numberDigits += (int)floor(log(data)/log(base)) + 1;
  PRINTF("number of digits is:%d\n", numberDigits);
  while(currentIndex < numberDigits)
  {  
    baseVal = (int)pow(base, numberDigits-1-currentIndex);  
    numVal = data / baseVal;  
    value = numVal + '0';  
    PRINTF("digit is: %c\n", value);
    *ptr  = value;
    ptr++;
    currentIndex++;
    data -= baseVal*numVal;
  }   
  *ptr = '\0'; 
  ptr-= currentIndex;
  PRINTF("%s\n", ptr);
 
return numberDigits;  

} 


int32_t my_atoi(uint8_t *ptr, uint8_t numDigits, uint32_t base)
{
  int32_t value = 0;
  uint8_t digit = 0;
  bool negativeNumber = false;
  
  PRINTF("%s\n", ptr);

  if(*ptr == '-')
  {
    negativeNumber= true;
    numDigits--;
    ptr++;
  }

  for(int i = 0; i < numDigits; i++)
  {
    digit = *ptr - '0';
    printf("%d\n", digit);
    value+=digit*(pow(base,numDigits - i -1));
    printf("%d\n", value);
    ptr++;
  }

  if(negativeNumber == true)
  {
   value*= -1;
  }

  return value;
}
