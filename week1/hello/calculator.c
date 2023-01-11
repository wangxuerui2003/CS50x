#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // prompt user for x
    long x = get_int("x: ");

    // prompt user for y
    long y = get_int("y: ");

    // print out the sum of x and y
    printf("x + y = %li\n", x+y);
}