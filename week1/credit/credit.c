#include <cs50.h>
#include <stdio.h>

int check(long card_num);
void card(long card_num);

int main(void)
{
    // get the card number
    long card_num = get_long("Card number: ");

    // check for validity (last digit of checksum is 0)
    int validity = check(card_num);
    if (validity == 1)
    {
        card(card_num);
    }
    else
    {
        printf("INVALID\n");
    }
}


int check(long card_num)
{
    int sum = 0;
    do
    {
        // get the odd position digit and add it to the final sum
        int odd_position_digit = card_num % 10;
        sum += odd_position_digit;

        // eliminate the current last digit for next operation to use
        card_num /= 10;

        // get the next even position digit
        int even_position_digit = card_num % 10;

        // double the even position digit
        int double_even_position_digit = even_position_digit * 2;

        // sum up the digits of the doubled digit and add to the final sum
        sum += (double_even_position_digit / 10) + (double_even_position_digit % 10);

        // eliminate the current last digit for the next loop to use
        card_num /= 10;
    }
    while (card_num != 0);

    // check the validity of the card (return 1 if valid, 0 if invalid)
    if (sum % 10 == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void card(long card_num)
{
    // check which kind of card this card number is
    // by checking both the number of digits the card number has
    // and also the first two or first digit(s) of the card
    int amex = card_num / 10000000000000;
    int mc = card_num / 100000000000000;
    int visa1 = card_num / 1000000000000;
    int visa2 = card_num / 1000000000000000;

    if (amex == 34 || amex == 37)
    {
        printf("AMEX\n");
    }

    else if (mc == 51 || mc == 52 || mc == 53 || mc == 54 || mc == 55)
    {
        printf("MASTERCARD\n");
    }
    else if (visa1 == 4 || visa2 == 4)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}