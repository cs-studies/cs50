#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize n infile outfile\n");
        return 1;
    }

    int factor = atoi(argv[1]);

    // validate factor value
    if (factor < 1 || factor > 100)
    {
        fprintf(stderr, "factor must be an positive integer less than or equal to 100\n");
        return 1;
    }

    char *infile = argv[2];
    char *outfile = argv[3];

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
    int in_bi_width = bi.biWidth;
    int in_bi_height = bi.biHeight;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    // set resized image width in pixels
    bi.biWidth *= factor;

    // set resized image height in pixels
    bi.biHeight *= factor;

    // determine padding for scanlines
    int in_padding = (4 - (in_bi_width * sizeof(RGBTRIPLE)) % 4) % 4;
    int out_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // set size of a resized image with padding in bytes
    bi.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + out_padding) * abs(bi.biHeight);

    // set size of a resized bitmap file
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    int abs_in_bi_height = abs(in_bi_height);
    RGBTRIPLE in_triples[abs_in_bi_height][in_bi_width];

    // parse infile image, start iterating over scanlines
    for (int h = 0; h < abs_in_bi_height; h ++)
    {
        // iterate over pixels in scanline
        for (int w = 0; w < in_bi_width; w ++)
        {
            // read RGB triple from infile
            fread(&in_triples[w][h], sizeof(RGBTRIPLE), 1, inptr);
        }
        fseek(inptr, in_padding, SEEK_CUR);
    }

    // write resulting image
    for (int h = 0, out_height = abs(bi.biHeight); h < out_height; h ++)
    {
        for (int w = 0; w < bi.biWidth; w ++)
        {
            // calculate position of a source RGB triple
            int w_source = floor(w / factor);
            int h_source = floor(h / factor);
            // write RGB triple into outfile
            fwrite(&in_triples[w_source][h_source], sizeof(RGBTRIPLE), 1, outptr);
        }
        for (int k = 0; k < out_padding; k ++)
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
