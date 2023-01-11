#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avrg = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Take average
            avrg = round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0);
            // Sub to every color
            image[i][j].rgbtRed = avrg;
            image[i][j].rgbtBlue = avrg;
            image[i][j].rgbtGreen = avrg;
        }
    }
    return;
}

// A function to limit the color under 255
int under_255(int num)
{
    if (num > 255)
    {
        return (255);
    }
    return (num);
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            // Limit under 255
            image[i][j].rgbtRed = under_255(round(0.393 * red + 0.769 * green + 0.189 * blue));
            image[i][j].rgbtGreen = under_255(round(0.349 * red + 0.686 * green + 0.168 * blue));
            image[i][j].rgbtBlue = under_255(round(0.272 * red + 0.534 * green + 0.131 * blue));
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Swap pixel
            RGBTRIPLE temp;
            temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Avoid writing repeated code
void    combine(int *temp, RGBTRIPLE pixel)
{
    temp[0] += pixel.rgbtRed;
    temp[1] += pixel.rgbtGreen;
    temp[2] += pixel.rgbtBlue;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE original[height][width];
    // Copy the image to a new 2d arr as a reference to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            original[i][j] = image[i][j];
        }
    }

    int   avrg[3];
    int count;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Take the original
            count = 0;
            avrg[0] = original[i][j].rgbtRed;
            avrg[1] = original[i][j].rgbtGreen;
            avrg[2] = original[i][j].rgbtBlue;
            // Take 4 edges
            if (i > 0)
            {
                combine(avrg, original[i - 1][j]);
                count++;
            }
            if (i < height - 1)
            {
                combine(avrg, original[i + 1][j]);
                count++;
            }
            if (j > 0)
            {
                combine(avrg, original[i][j - 1]);
                count++;
            }
            if (j < width - 1)
            {
                combine(avrg, original[i][j + 1]);
                count++;
            }
            // Take the corners
            if (i > 0 && j > 0)
            {
                combine(avrg, original[i - 1][j - 1]);
                count++;
            }
            if (i < height - 1 && j < width - 1)
            {
                combine(avrg, original[i + 1][j + 1]);
                count++;
            }
            if (j > 0 && i < height - 1)
            {
                combine(avrg, original[i + 1][j - 1]);
                count++;
            }
            if (i > 0 && j < width - 1)
            {
                combine(avrg, original[i - 1][j + 1]);
                count++;
            }
            // Change the image
            image[i][j].rgbtRed = round(avrg[0] / (float)(count + 1));
            image[i][j].rgbtGreen = round(avrg[1] / (float)(count + 1));
            image[i][j].rgbtBlue = round(avrg[2] / (float)(count + 1));
        }
    }
    return;
}