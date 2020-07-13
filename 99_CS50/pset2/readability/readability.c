//This algorithm takes users text and calculates the Coleman-Liau index.
//The Coleman-Liau index of a text is designed to output what (U.S.) grade level is needed to understand the text.
//The formula is: index = 0.0588 * L - 0.296 * S - 15.8

//includes.
#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

//declare variables.
float letters = 0;
float words = 1;
float sentences = 0;

//start algorithm.
int main(void)
{
    //prompt user for text and calc length of string.
    string text = get_string("Text: ");
    size_t n = strlen(text);

    //iterates through each character, counting letters, words and sentences.
    for (int i = 0; i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
        if (isspace(text[i]))
        {
            words++;
        }
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }

    //calculates variables l and s for Coleman-Liau index.
    float l = letters / words * 100;
    float s = sentences * 100 / words;
    int grade = round(0.0588 * l - 0.296 * s - 15.8);

    //prints grade value with conditions where: (1 > grade >= 16).
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}