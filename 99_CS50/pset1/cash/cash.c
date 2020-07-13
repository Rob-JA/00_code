//get libraries
#include <stdio.h>
#include <cs50.h>
#include <math.h>

//start script
int main(void)
{

    //introduce variables
    float dollars;
    int coins = 0;

    //prompt user for required change, validate for number > 0.
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0);

    //convert dollars to cents and round.
    int cents = round(dollars * 100);

    //check for 25c.
    while (cents - 25 >= 0)
    {
        coins++;
        cents = cents - 25;
    }

    //check for 10c.
    while (cents - 10 >= 0)
    {
        coins++;
        cents = cents - 10;
    }

    //check for 5c.
    while (cents - 5 >= 0)
    {
        coins++;
        cents = cents - 5;
    }

    //check for 1c.
    while (cents - 1 >= 0)
    {
        coins++;
        cents = cents - 1;
    }

    //return number of coins required.
    printf("%i\n", coins);

}