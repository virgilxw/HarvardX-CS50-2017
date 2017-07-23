/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }


    // remember CLIs
    char *infile = argv[1];

    FILE *inptr = fopen(infile, "r");
    fseek(inptr, SEEK_SET, 0);
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    char fileName[8];
    int fileNo = 0;
    sprintf(fileName, "%03i.jpg", fileNo);
    BYTE buffer[512] = {0};
    
    FILE *outptr = fopen(fileName, "w");
    if (outptr == NULL)
    {
        printf("Error! Could create the file!\n");
        return 3;
    }
    
    
    while(fread(buffer, 512, 1, inptr))
    {
        
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (fileNo > 0)
            {
                fclose(outptr);
            }
            
            sprintf(fileName, "%03d.jpg", fileNo);
            outptr = fopen(fileName, "w");
            if (outptr == NULL)
            {
                printf("Error! Could create the file!\n");
                return 3;
            }
            

            fileNo++;
        }
        
        if (fileNo > 0)
        {
            fwrite(buffer, 512, 1, outptr);
        }
    }
    
    fclose(inptr);
    fclose(outptr);
    
    return 0;
}
