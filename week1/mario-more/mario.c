#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // define an integer height
    int height;

    // if the height is lower than 1 or higher than 8, then ignore the height
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // print out the bricks using loops
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < height * 2 + 2; j++)
        {
            if (height - j <= i + 1 && height - j > 0)
            {
                printf("#");
            }
            else if (height - j == 0 || height - j == -1)
            {
                printf(" ");
            }
            else if (height - j > i + 1)
            {
                printf(" ");
            }
            else if (height - j >= - i - 2)
            {
                printf("#");
            }
        }

        // a new line after each row
        printf("\n");
    }
}