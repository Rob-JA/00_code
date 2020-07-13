//Algorithm requires command line argument (key) to be entered after ./caesar (an integer + or -)
//Then the user is prompted for a string of text.
//The algorithm then will add the key to each letter of the code and output cipherte

//includes.
#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//declare variables.
int ciphertext;
int diff;

//declare function.
int plaintext_to_ciphertext(int upperbound, int lowerbound, int a, int k);

//start algorithm.
int main(int argc, string argv[])
{

    //checks if the correct number of arguments is entered (1)
    if (argc != 2)
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }

    //checks the length of argument string.
    int k = atoi(argv[1]);
    size_t z = strlen(argv[1]);

    //iterates through argument to check for non numerical values.
    for (int j = 0; j < z; j++)
    {
        if (isdigit(argv[1][j]) == 0)
        {
            printf("Usage: %s key\n", argv[0]);
            return 1;
        }
    }

    //prompts user for plaintext and calcs length of string.
    string plaintext = get_string("plaintext: ");
    size_t n = strlen(plaintext);
    printf("ciphertext: ");

    //iterates through array of string values.
    //calls upon plaintext_to_ciphertext fuction and sets bounds based on upper or lowercase.
    for (int i = 0; i < n; i++)
    {
        if (isupper(plaintext[i]))
        {
            int upperbound = 90;
            int lowerbound = 65;
            int a = plaintext[i];
            ciphertext = plaintext_to_ciphertext(upperbound, lowerbound, a, k);
            printf("%c", ciphertext);
        }
        else if (islower(plaintext[i]))
        {
            int upperbound = 122;
            int lowerbound = 97;
            int a = plaintext[i];
            ciphertext = plaintext_to_ciphertext(upperbound, lowerbound, a, k);
            printf("%c", ciphertext);
        }
        else
        {
            printf("%c", plaintext[i]);
        }
    }
    printf("\n");
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
////FUNCTIONS/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

// plaintext to ciphertext function.
int plaintext_to_ciphertext(int upperbound, int lowerbound, int a, int k)
{
    ciphertext = a + k;

    while (ciphertext > upperbound)
    {
        diff = ciphertext - upperbound;
        ciphertext = lowerbound - 1 + diff;
    }

    while (ciphertext < lowerbound)
    {
        diff = ciphertext - lowerbound;
        ciphertext = upperbound + 1 + diff;
    }
    return ciphertext;
}