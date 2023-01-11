#include <stdio.h>
#include <cs50.h>

float discount(float price, int percent_off);

int main(void)
{
    float regular = get_float("regular price: ");
    int percent_off = get_float("percent off: ");
    printf("after %i percentage discount: %.2f\n", percent_off, discount(regular, percent_off));
}

float discount(float price, int percent_off)
{
    return price * (100 - percent_off) / 100;
}