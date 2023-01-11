#include <cs50.h>
#include <stdio.h>

int get_cents(void);
int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void)
{
    // Ask how many cents the customer is owed
    int cents = get_cents();

    // Calculate the number of quarters to give the customer
    int quarters = calculate_quarters(cents);
    cents = cents - quarters * 25;

    // Calculate the number of dimes to give the customer
    int dimes = calculate_dimes(cents);
    cents = cents - dimes * 10;

    // Calculate the number of nickels to give the customer
    int nickels = calculate_nickels(cents);
    cents = cents - nickels * 5;

    // Calculate the number of pennies to give the customer
    int pennies = calculate_pennies(cents);
    cents = cents - pennies * 1;

    // Sum coins
    int coins = quarters + dimes + nickels + pennies;

    // Print total number of coins to give the customer
    printf("%i\n", coins);
}

int get_cents(void)
{
    int cents;
    do
    {
        // get how many cents the cashier owes
        cents = get_int("Change owed: ");
    }
    while (cents < 0);
    return cents;

}

int calculate_quarters(int cents)
{
    // Use division to get the maximum number of quarters that cashier can give
    int quarters = cents / 25;
    return quarters;
}

int calculate_dimes(int cents)
{
    // Use division to get the maximum number of dimes that cashier can give
    int dimes = cents / 10;
    return dimes;
}

int calculate_nickels(int cents)
{
    // Use division to get the maximum number of nickels that cashier can give
    int nickels = cents / 5;
    return nickels;
}

int calculate_pennies(int cents)
{
    // Use division to get the maximum number of pennies that cashier can give
    int pennies = cents / 1;
    return pennies;
}
