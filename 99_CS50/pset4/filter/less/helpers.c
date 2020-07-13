#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //iterates through each pixel.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //calculates average.
            float avg = ((float) image[i][j].rgbtRed + (float) image[i][j].rgbtGreen + (float) image[i][j].rgbtBlue) / 3.0;

            //rounds average.
            avg = round(avg);

            //casts average as an integer.
            avg = (int) avg;

            //sets colours to average.
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //iterates through each pixel.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //introduces temp variables and sets them to a float (0.000).
            float originalRed = image[i][j].rgbtRed;
            float originalGreen = image[i][j].rgbtGreen;
            float originalBlue = image[i][j].rgbtBlue;

            //calculate sepia values.
            float sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue;
            float sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue;
            float sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue;

            //checks if value exceeds max value of 255.
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            //rounds sepia values.
            sepiaRed = round(sepiaRed);
            sepiaGreen = round(sepiaGreen);
            sepiaBlue = round(sepiaBlue);

            //sets sepia colours and casts as an integer.
            image[i][j].rgbtRed = (int) sepiaRed;
            image[i][j].rgbtGreen = (int) sepiaGreen;
            image[i][j].rgbtBlue = (int) sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //iterates through each pixel.
    for (int i = 0; i < height; i++)
    {
        //
        int a;
        if (width % 2 == 0)
        {
            a = (width / 2) - 1;
        }
        else
        {
            a = (width - 1) / 2;
        }

        for (int j = 0; j <= a; j++)
        {
            int k = (width - 1) - j;
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][k];
            image[i][k] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create copy of immage/array.
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    //iterate throough each pixel of image.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            //introduce limit variables.
            int a;
            int b;
            int c;
            int d;

            //set vertical limits.
            if (i == 0)
            {
                a = 0;
                c = 1;
            }
            else if (i == height - 1)
            {
                a = 1;
                c = 0;
            }
            else
            {
                a = 1;
                c = 1;
            }

            //set horizontal limits.
            if (j == 0)
            {
                b = 0;
                d = 1;
            }
            else if (j == width - 1)
            {
                b = 1;
                d = 0;
            }
            else
            {
                b = 1;
                d = 1;
            }

            float count = 0.0;
            float avgRed = 0;
            float avgGreen = 0;
            float avgBlue = 0;

            //iterate through each pixel around the pixel to be blurred. Summing RBG colours and counting no. of pixels.
            for (int y = i - a; y <= i + c; y++)
            {
                for (int x = j - b; x <= j + d; x++)
                {
                    avgRed += image[y][x].rgbtRed;
                    avgGreen += image[y][x].rgbtGreen;
                    avgBlue += image[y][x].rgbtBlue;
                    count++;
                }
            }

            //calculate average, round and cast as integer. apply to temp image.
            temp[i][j].rgbtRed = (int) round(avgRed / count);
            temp[i][j].rgbtGreen = (int) round(avgGreen / count);
            temp[i][j].rgbtBlue = (int) round(avgBlue / count);
        }
    }

    //copy new image to original image.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}
