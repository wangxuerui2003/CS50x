#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int reading_index(string text);

int main(void)
{
    // get the text from user
    string text = get_string("Text: ");

    // determine the grade of readability
    int index = reading_index(text);
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

}

int reading_index(string text)
{
    // define the variables
    int spaces = 0;
    int punctuations = 0;
    int letters = 0;
    int length = strlen(text);

    // loop through the string and check if the char is a space or a punctuation
    for (int i = 0; i < length; i++)
    {
        if (text[i] == ' ')
        {
            spaces += 1;
        }
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            punctuations += 1;
        }
        else if (tolower(text[i]) >= 'a' && tolower(text[i]) <= 'z')
        {
            letters += 1;
        }
    }

    // find the readability index
    int words = spaces + 1;
    int sentences = punctuations;
    float L = letters / (float) words * 100;
    float S = sentences / (float) words * 100;
    float readability = 0.0588 * L - 0.296 * S - 15.8;

    // round the index to the nearest int and return it
    // if ((int) (readability + 0.5) == (int) readability)
    // {
    //     return (int) readability;
    // }
    return (int) (readability + 0.5);
}