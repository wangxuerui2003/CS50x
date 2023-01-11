#include "helpers.h"
#include <math.h>

typedef struct
{
    int g_red;
    int g_green;
    int g_blue;
}   g_value;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int sum;

    // Take the sum of R, G, B value and take an average and reset R, G, B to the average
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sum = image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue;
            image[i][j].rgbtRed = round((float)sum / (float)3);
            image[i][j].rgbtGreen = round((float)sum / (float)3);
            image[i][j].rgbtBlue = round((float)sum / (float)3);
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE   temp;

    // Swap the pixel and position j in a row with the position (width - j) in a row
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE   temp[height][width];
    int         sum_red;
    int         sum_green;
    int         sum_blue;
    int         count;

    // Copy the image to temp
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    // This is a comment
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            count = 0;
            sum_red = 0;
            sum_green = 0;
            sum_blue = 0;
            // Take the sums of RGB values
            for (int r = -1; r < 2; r++)
            {
                for (int c = -1; c < 2; c++)
                {
                    if (i + r < 0 || i + r >= height)
                    {
                        continue;
                    }
                    if (j + c < 0 || j + c >= width)
                    {
                        continue;
                    }
                    sum_red += temp[i + r][j + c].rgbtRed;
                    sum_green += temp[i + r][j + c].rgbtGreen;
                    sum_blue += temp[i + r][j + c].rgbtBlue;
                    count++;
                }
            }
            // Take the average for eacg R, G, B value
            image[i][j].rgbtRed = round((float)sum_red / (float)count);
            image[i][j].rgbtGreen = round((float)sum_green / (float)count);
            image[i][j].rgbtBlue = round((float)sum_blue / (float)count);
        }
    }
}

// Detect and highlight edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Make a copy of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    // Compute Gx and Gy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int gxBlue = 0;
            int gyBlue = 0;
            int gxGreen = 0;
            int gyGreen = 0;
            int gxRed = 0;
            int gyRed = 0;

            for (int r = -1; r < 2; r++)
            {
                for (int c = -1; c < 2; c++)
                {
                    if (i + r < 0 || i + r > height - 1)
                    {
                        continue;
                    }
                    if (j + c < 0 || j + c > width - 1)
                    {
                        continue;
                    }

                    // This is a comment
                    gxRed += temp[i + r][j + c].rgbtRed * gx[r + 1][c + 1];
                    gyRed += temp[i + r][j + c].rgbtRed * gy[r + 1][c + 1];
                    gxGreen += temp[i + r][j + c].rgbtGreen * gx[r + 1][c + 1];
                    gyGreen += temp[i + r][j + c].rgbtGreen * gy[r + 1][c + 1];
                    gxBlue += temp[i + r][j + c].rgbtBlue * gx[r + 1][c + 1];
                    gyBlue += temp[i + r][j + c].rgbtBlue * gy[r + 1][c + 1];
                }
            }

            // Take sqrt of square mean of gx and gy
            int blue = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));
            int green = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            int red = round(sqrt(gxRed * gxRed + gyRed * gyRed));

            // Edit the original image
            image[i][j].rgbtBlue = (blue > 255) ? 255 : blue;
            image[i][j].rgbtGreen = (green > 255) ? 255 : green;
            image[i][j].rgbtRed = (red > 255) ? 255 : red;
        }
    }
}