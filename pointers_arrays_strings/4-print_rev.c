#include "main.h"

/**
 * print_rev - print in reverse a string
 * @s: takes a string of characters
 */
void print_rev(char *s)
{
    int len = 0;
    int index = 0; /* Initialize index to 0 */

    while (s[index++])
        len++;

    for (index = len - 1; index >= 0; index--)
        _putchar(s[index]);

    _putchar('\n');
}
