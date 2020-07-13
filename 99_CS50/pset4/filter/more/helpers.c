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

// Detect edges

// Pseudo Code
//
// 1. Create a copy of the image.
// 2. Create the Gx array.
// 3. Create the Gy array.
// 4. Cycle through the pixels of the og image, getting the red, green, blue values.
// 5. For each colour value in each pixel around the image pixel in question calculate the Gx value.
//   i. Cycle through Gx array with the pixels surrounding the image pixel in question.
//   ii. Sum the Gx value.
// 6. For each colour value in each pixel calculate the Gy value.
//   i. Cycle through Gy array with the pixels surrounding the image pixel in question.
//   ii. Sum the Gy value.
// 7. Using the Sobel filter algorithm, caluclate the Sobel operator (new colour).
// 8. Round and cap this number at 255.
// 9. Apply edge cases.


void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int gxarray[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gyarray[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    //create copy of immage/array.
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    //iterate through each pixel.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            float gxRed = 0;
            float gxGreen = 0;
            float gxBlue = 0;
            float gyRed = 0;
            float gyGreen = 0;
            float gyBlue = 0;
            int n = 0;

            //for each pixel iterate in the 3x3 square around the pixel.
            for (int a = i - 1; a <= i + 1; a++)
            {
                for (int b = j - 1; b <= j + 1; b++, n++)
                {
                    
                    //set limits for edge cases.
                    if (a < 0 || a > height - 1 || b < 0 || b > width - 1)
                    {
                        
                    }
                    
                    //sum Gx and Gy multiplying colour values by gx/gy arrays.
                    else
                    {
                        gxRed += gxarray[n] * image[a][b].rgbtRed;
                        gxGreen += gxarray[n] * image[a][b].rgbtGreen;
                        gxBlue += gxarray[n] * image[a][b].rgbtBlue;

                        gyRed += gyarray[n] * image[a][b].rgbtRed;
                        gyGreen += gyarray[n] * image[a][b].rgbtGreen;
                        gyBlue += gyarray[n] * image[a][b].rgbtBlue;
                    }
                }
            }

            //calculate the sobel operator for each colour (Sobel Operator = sqrt(Gx^2 + Gy^2)) (SO is simply the new colour).
            int sobelRed = (int) round(sqrtf((gxRed * gxRed) + (gyRed * gyRed)));
            int sobelGreen = (int) round(sqrtf((gxGreen * gxGreen) + (gyGreen * gyGreen)));
            int sobelBlue = (int) round(sqrtf((gxBlue * gxBlue) + (gyBlue * gyBlue)));

            //cap colour at 255.
            if (sobelRed > 255)
            {
                sobelRed = 255;
            }
            if (sobelGreen > 255)
            {
                sobelGreen = 255;
            }
            if (sobelBlue > 255)
            {
                sobelBlue = 255;
            }

            //apply colour to temporary array.
            temp[i][j].rgbtRed = sobelRed;
            temp[i][j].rgbtGreen = sobelGreen;
            temp[i][j].rgbtBlue = sobelBlue;
        }
    }

    //copy temp array to original array.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}
