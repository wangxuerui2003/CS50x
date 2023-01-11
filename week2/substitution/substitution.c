#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // check whether the number of command line arguments is 2
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // check whether the lenght of the key is 26
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }

    // check if there is repeated cheracters or non-alphabetical values in the key
    for (int i = 0, length = strlen(argv[1]); i < length; i++)
    {
        if (isalpha(argv[1][i]))
        {
            for (int j = 0; j < length; j++)
            {
                if (j != i && isalpha(argv[1][j]))
                {
                    if (tolower(argv[1][j]) == tolower(argv[1][i]))
                    {
                        printf("Key must not contain repeated characters\n");
                        return 1;
                    }
                }
            }
        }
        else
        {
            printf("Key must only contain alphabetical characters\n");
            return 1;
        }
    }

    // get the key and plain text
    string key = argv[1];
    string text = get_string("plain text: ");

    // substitute every alphabetical characters to their corresponding key and case
    for (int k = 0, length = strlen(text); k < length; k++)
    {
        // check if the character is a letter
        if (isalpha(text[k]))
        {
            // check the case of the letter and convert it according to the case
            if (isupper(text[k]))
            {
                text[k] = toupper(key[text[k] - 65]);
            }
            else
            {
                text[k] = tolower(key[text[k] - 97]);
            }
        }
    }

    // output the ciphertext
    printf("ciphertext: %s\n", text);
}