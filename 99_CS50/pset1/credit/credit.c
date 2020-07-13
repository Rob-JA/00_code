//get libraries
#include <stdio.h>
#include <cs50.h>
#include <math.h>

//start algorithm
int main(void)
{

    //introduce variables.
    int counter;            //counter will be used to count number of digits.
    int checksum1 = 0;      //checksum1 will be the odd numbers starting with last digit. (1st, 3rd, 5th...)
    int checksum2 = 0;      //checksum2 will be every other number starting with 2nd to last. (2nd, 4th, 6th...)

    //prompt user for card number
    long ccnumber = get_long("Number: ");
    long number = ccnumber;

    //this iterates through card digits, adding numbers to either checksum1 or checksum2,
    //also functions as a digit counter.
    for (counter = 0; number / 10 > 0 || number % 10 > 0; counter++)
    {
        int odd = number % 10;
        number = number / 10;
        checksum1 = checksum1 + odd;

        if (number / 10 > 0 || number % 10 > 0)
        {
            int even = number % 10 * 2;
            int integer = even / 10;
            int remain = even % 10;
            even = integer + remain;

            checksum2 = checksum2 + even;
            number = number / 10;
            counter++;
        }
    }

    //isolates the last 2 digits.
    for (int i = 1; i < counter - 1; i++)
    {
        ccnumber = ccnumber / 10;
    }

    //tests checksum condition.
    if ((checksum1 + checksum2) % 10 != 0)
    {
        printf("INVALID\n");
    }

    //tests amex conditions.
    else if ((counter == 15) && (ccnumber == 34 || ccnumber == 37))
    {
        printf("AMEX\n");
    }
    
    //tests mastercard conditions.
    else if ((counter == 16) && (ccnumber >= 51 && ccnumber <= 55))
    {
        printf("MASTERCARD\n");
    }
    
    //tests visa conditions. further isolates last digit.
    else if ((counter == 13 || counter == 16) && (ccnumber / 10 == 4))
    {
        printf("VISA\n");
    }

    else
    {
        printf("INVALID\n");
    }
}