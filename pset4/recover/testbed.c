/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./testbed file\n");
        return 1;
    }
    
    
    char *infile = argv[1];
    
    FILE *inptr = fopen(infile, "r");
    
    
    fseek(inptr, 0L, SEEK_END); 
    int size = ftell(inptr);
    
    
    printf("Size of infile: %d\n", size);
    
    
    fclose(inptr);
    
}
