#include <stdio.h>

/**
 * main - Finds and prints the largest prime
 *        factor of the number 612852475143.
 *
 * Return: Always 0.
 */
int main(void)
{
    long prime = 612852475143;
    long div = 2;

    while (div * div <= prime)
    {
        if (prime % div == 0)
        {
            prime /= div;
        }
        else
        {
            div++;
        }
    }

    printf("%ld\n", prime);

    return (0);
}
