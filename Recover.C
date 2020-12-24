#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{

    typedef uint8_t BYTE;
    BYTE buffer[512];
    int counter = 0;
    char filename[8];
    FILE *image;

    //check that user input is correct
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // remember filename
    char *memory_card = argv[1];

    // open memory card file
    FILE *memcardfile = fopen(memory_card, "r");
    if (memcardfile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", memory_card);
        return 2;
    }
    //repeat until end of memory card
    while (fread(buffer, sizeof(BYTE), 512, memcardfile))
    {
        //check if the first four bytes of the 512 byte chunk of memory indicate the beginning of a jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //check to see if this is the first jpeg
            if (counter == 0)
            {
                //create and open new jpeg file
                sprintf(filename, "%03i.jpg", counter);
                image = fopen(filename, "w");
                //copy the jpeg from the memory card into the new image file
                fwrite(buffer, 512, 1, image);
                //increment the counter
                counter++;
            }
            else
            {
                fclose(image);
                sprintf(filename, "%03i.jpg", counter);
                image = fopen(filename, "w");
                fwrite(buffer, 512, 1, image);
                counter++;
            }
        }
        else
        {
            //continue writing until a new jpeg has been detected
            fwrite(buffer, 512, 1, image);
        }


    }
    fclose(image);
    fclose(memcardfile);
}

