#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Asks users name
    string name = get_string("What is your name?\n");
    
    //Says hello using the users name
    printf("hello, %s\n", name);
}