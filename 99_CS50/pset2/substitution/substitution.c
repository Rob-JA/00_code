//Algorithm requires command line argument key to be entered after ./substitution 
//key is to be a jumbled up alphabet. the user will then be prompted for plain text.
//the script will then substitute the normal alphabet from the key alphabet characters and cipher text accordingly.

//includes.
#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//declare variables.

//declare function.


//start algorithm.
int main(int argc, string argv[])
{
    //checks if the correct number of arguments is entered (2)
    if (argc != 2)
    {
        printf("Usage: %s KEY\n", argv[0]);
        return 1;
    }

    size_t z = strlen(argv[1]);

    //checks the length of key string.
    if (z != 26)
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }

    int s = z;

    //iterates through key to check for non alphabetical values.
    for (int j = 0; j < z; j++, s--)
    {
        if (isalpha(argv[1][j]) == 0)
        {
            printf("Key must only contain alphabetic characters.\n");
            return 1;
        }

        for (int k = 1; k < s; k++)
        {
            int y = argv[1][j];
            int x = argv[1][j + k];
            if (x == y)
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
        }
    }

    //prompts user for plaintext and calcs length of string.
    string plaintext = get_string("plaintext: ");
    printf("ciphertext: ");

    size_t length = strlen(plaintext);

    //substituion with the key. handles whether key is + or -
    for (int a = 0; a < length; a++)
    {
        if (isupper(plaintext[a]))
        {
            int b = plaintext[a] - 65;
            if (isupper(argv[1][b]))
            {
                printf("%c", argv[1][b]); 
            }
            if (islower(argv[1][b]))
            {
                printf("%c", argv[1][b] - 32);  
            }
        }
        else if (islower(plaintext[a]))
        {
            int b = plaintext[a] - 97;
            if (isupper(argv[1][b]))
            {
                printf("%c", argv[1][b] + 32); 
            }
            if (islower(argv[1][b]))
            {
                printf("%c", argv[1][b]);  
            }
        }
        else
        {
            printf("%c", plaintext[a]);
        }
    }
    printf("\n");
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
////FUNCTIONS/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

