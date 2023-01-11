#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // check if the user input the correct command line arguments
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        // return an exit value to the main function
        return 1;
    }

    for (int j = 0, length = strlen(argv[1]); j < length; j++)
    {
        if (isdigit(argv[1][j]))
        {
            continue;
        }
        else
        {
            printf("Usage: ./caesar key\n");
            //return an exit value to the main function
            return 1;
        }
    }

    // ge the key and the text
    int key = atoi(argv[1]);
    string text = get_string("plaintext: ");

    // convert all the alphabets to its corresponding cipher letter
    for (int i = 0, length = strlen(text); i < length; i++)
    {
        if (isalpha(text[i]))
        {
            // use different formula for capital and lower letters
            if (isupper(text[i]))
            {
                int alpha_index = text[i] - 65;
                text[i] = (alpha_index + key) % 26 + 65;
            }
            else
            {
                int alpha_index = text[i] - 97;
                text[i] = (alpha_index + key) % 26 + 97;
            }
        }
    }

    // output the ciphertext
    printf("ciphertext: %s\n", text);
}