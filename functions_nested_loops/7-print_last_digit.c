#include "main.h"
/**
* print_last_digit - a function that prints the last digit of a number
*
* @c: a character
* Return: Always (0)
*/
int print_last_digit(int c)
{
int last;

last = c % 10;

if (last < 0)
{
last = last * -1;
}

_putchar(last + '0');

return (last);
}
