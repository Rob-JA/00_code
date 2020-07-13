// recover.c
//------------------------------------------
// each jpeg is contained within a 512 block
// starts with 0xff 0xd8 0xff 0xe" "
// will recover "lost" jpeg images and write them to a new jpeg file.
//------------------------------------------
//
//  pesudo code
//  -----------
//  open memory card
//  repeat until end of card
//      read 512 bytes into a buffer
//      if start of new jpeg using fread
//          if first jpeg
//              write to new file
//          else
//              close current file and open a new file to write to
//      else
//          continue writing to file
//  close any remaining files
//
//------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc != 2)
    {
        printf("Usage: ./recovery image\n");
        return 1;
    }

    //introduce counter to count no. of files.
    int counter = 0;

    //ensure string has enough memory to store entire filename in format "000.jpg\0".
    char filename[8];

    //open card.
    FILE *file = fopen(argv[1], "r");

    //file pointer to write to.
    FILE *img = NULL;

    //declare buffer array to store temp 512 bits of jpeg file.
    BYTE buffer[512];

    //write to buffer array. when unable to stop (!= 0).
    while (fread(buffer, sizeof(buffer), 1, file) == 1)
    {
        //check if start of new jpeg file.
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //if its not the first file then close open file (otherwise it'll open them all)
            if (counter > 0)
            {
                fclose(img);
            }

            //name output file.
            sprintf(filename, "%03i.jpg", counter);
            counter++;

            //open new file named with new filename "###.jpeg\0"
            img = fopen(filename, "w");

            //wite to output file.
            fwrite(buffer, sizeof(buffer), 1, img);
            //data: pointer to bytes that will be written to file
            //size: size of each element to write
            //number: number of elements to write
            //outptr: FILE * to write to
        }
        
        //if not the first file then just continue writing to buffer.
        else if (counter > 0)
        {
            //write to output file.
            fwrite(buffer, sizeof(buffer), 1, img);
        }
    }
    
    //close files.
    fclose(img);
    fclose(file);
    return 0;
}