#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int is_red(RGBTRIPLE);
int is_white(RGBTRIPLE);

int main(int argc, char *argv[])
{
    // Ensure correct usage
    if (argc != 3)
    {
        fprintf(stderr, "Usage: whodunit INPUT_FILE OUTPUT_FILE\n");
        return 1;
    }

    // Save input and output files names
    char *infile = argv[1];
    char *outfile = argv[2];

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // Create an output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // Read input file BITMAPFILEHEADER: type, size, layout of a file
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // Read input file BITMAPINFOHEADER: dimensions and color format of a file
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Ensure infile is a 24-bit uncompressed BMP 4.0
    // Check file type, headers size, bits-per-pixel, compression type
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 ||
        bi.biSize != 40 || bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    // Write output file BITMAPFILEHEADER, it is the same as of inputfile
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // Write output file BITMAPINFOHEADER, it is the same as of inputfile
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Calculate scanlines padding, the resulting scanline size must be a multiple of 4
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Iterate over rows (scanlines)
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i ++)
    {
        // Iterate over columns (pixels)
        // bi.biWidth doesn't include padding. Omit padding, as it isn't a RGBTRIPLE
        for (int j = 0; j < bi.biWidth; j ++)
        {
            RGBTRIPLE triple;

            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            if (is_red(triple))
            {
                // Make this triple white
                triple.rgbtBlue = 0xff;
                triple.rgbtGreen = 0xff;
            }
            if (!is_white(triple))
            {
                // Make this triple black
                triple.rgbtBlue = triple.rgbtGreen = triple.rgbtRed = 0x00;
            }

            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // Skip padding on reading
        fseek(inptr, padding, SEEK_CUR);

        // Add padding to an ouptut file
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }

    }

    // Close files
    fclose(outptr);
    fclose(inptr);

    return 0;
}

int is_red(RGBTRIPLE triple)
{
    return triple.rgbtBlue == 0x00 && triple.rgbtGreen == 0x00 && triple.rgbtRed == 0xff;
}

int is_white(RGBTRIPLE triple)
{
    return triple.rgbtBlue == 0xff  && triple.rgbtGreen == 0xff && triple.rgbtRed == 0xff;
}