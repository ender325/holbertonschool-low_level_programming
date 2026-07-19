#include "main.h"
/**
 * _isalpha - function that checks for alphabetic character
 * @c: the characted to be checked
 *
 * Return: 1 lower/uppercase, 0 otherwise
 */

int _isalpha(int c)
{
if (c >= 97 && c <= 122)
{
return (1);
}
else if (c >= 65 && c <= 90)
{
return (1);
}
else
{
return (0);
}
}
