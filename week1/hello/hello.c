#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // get the user's name
    string name = get_string("What is your name? ");
    // print out the greeting message
    printf("hello, %s\n", name);
    printf("Nice to meet you!\n");
}