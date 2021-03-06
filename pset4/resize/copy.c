/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy factor infile outfile\n");
        return 1;
    }

    // remember CLIs
    double factor = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];
    
    // Check if factor is within range
    if (factor < 0 || factor > 100)
    {
        fprintf(stderr, "factor not between 0 and 100");
        return 5;
    }

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // determine padding for old scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //Create RGBTRIPLE array
    RGBTRIPLE RGBArray[bi.biWidth][abs(bi.biHeight)];
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            
            // read RGB triple from infile
            fread(&RGBArray[i][j], sizeof(RGBTRIPLE), 1, inptr);
            
        }
        
        fseek(inptr, padding, SEEK_CUR);
    }


    // determine padding for new scanlines
    padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // change header information
    bi.biWidth = round(bi.biWidth * factor);
    bi.biHeight = round(bi.biHeight * factor);
    bi.biSizeImage = round(50 + (bi.biWidth * abs(bi.biHeight)) + padding);
    bf.bfSize = bi.biSizeImage + 54;
    

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            //Figure out which pixel should be duplicated and write it into triple
            triple = RGBArray[ (int) round(i/factor) ][(int) round(j/factor)];

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // add padding
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
