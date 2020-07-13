//get libraries
#include <stdio.h>
#include <cs50.h>


//start script
int main(void)

{
    //introduce user entered variable
    int height;


    //prompt user for height and validate (1 < height < 8)
    do
    {
        height = get_int("Height: ");
    }
    while (height > 8 || height < 1);


    //draw right adjusted pyramid with a gap and then a left adjusted pyramid
    //add in new row
    for (int i = 1; i <= height; i++)
    {
        //add in spaces
        for (int n = height - i; n > 0; n--)
        {
            printf(" ");
        }

        //add in left side hashes
        for (int j = 1; j <= i; j++)
        {
            printf("#");
        }
        
        //add in gap
        printf("  ");
        
        //add in right side hashes
        for (int j = 1; j <= i; j++)
        {
            printf("#");
        }
        
        printf("\n");
    }
}