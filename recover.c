#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    int jpegCount = 0;
    BYTE buffer[512];
    char filename[8];
    FILE *img = NULL;

    if(argc != 2)
    {
        return 1;
    }

    FILE *memoryCard = fopen(argv[1], "r");
        if (memoryCard != NULL)
        {
            while (fread(&buffer, sizeof(BYTE), 512, memoryCard) == 512)
            {
                //if first byte contains a header for a jpeg, it means new picture found -> new jpeg file
                if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
                {

                    if (jpegCount == 0) //If first jpeg
                    {
                        sprintf(filename, "%03i.jpg", jpegCount);
                        img  = fopen(filename, "w");
                        fwrite(&buffer, sizeof(BYTE), 512, img);
                    }
                    else //If not first jpeg
                    {
                        fclose(img);
                        sprintf(filename, "%03i.jpg", jpegCount);
                        img  = fopen(filename, "w");
                        fwrite(&buffer, sizeof(BYTE), 512, img);
                        printf("%s\n", filename);
                    }
                    jpegCount++;
                }
                else
                {
                    if (jpegCount != 0)
                    {
                        fwrite(&buffer, sizeof(BYTE), 512, img);
                    }
                }
            }
            fclose(img);
            fclose(memoryCard);
        }

}