#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;

int main(int ac, char **av)
{
    FILE    *card_ptr;
    FILE    *img_ptr;
    BYTE    buffer[512];
    int     counter;
    char    filename[8];

    // Input checking
    if (ac != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return (1);
    }
    // This is a comment
    card_ptr = fopen(av[1], "r");
    counter = 0;
    while (true)
    {
        // Check if has read until the end of the card
        if (fread(buffer, sizeof(BYTE), 512, card_ptr) != 512)
        {
            fclose(img_ptr);
            fclose(card_ptr);
            return (0);
        }
        // This is a comment
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (counter != 0)
            {
                fclose(img_ptr);
            }
            // Generate filename
            sprintf(filename, "%03i.jpg", counter++);
            img_ptr = fopen(filename, "w");
            fwrite(buffer, sizeof(BYTE), 512, img_ptr);
        }
        else
        {
            // If not jpg yet, skip the buffer
            if (counter == 0)
            {
                continue ;
            }
            // This is a comment
            fwrite(buffer, sizeof(BYTE), 512, img_ptr);
        }
    }
    return (0);
}