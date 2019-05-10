#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BLOCK_SIZE 512

int is_image_start(unsigned char *buffer);

int main(int argc, char *argv[])
{
    // Validate user entered correct command
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // Open file with the raw data
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Failed to open file %s, errno %d\n", argv[1], errno);
        return 1;
    }

    // Allocate memory to store blocks of data from the raw data file.
    // BLOCK_SIZE may be a large value, so use malloc to allocate memory on the heap.
    unsigned char *buffer = malloc(BLOCK_SIZE);
    if (!buffer)
    {
        fprintf(stderr, "Failed to allocate memory for buffer\n");
        return 2;
    }

    // Initialize variablle to store image file name.
    /// The memory will be allocated on the stack.
    char image_name[8] = {0};

    // Initialize file pointer with NULL, to identify later if a file is opened
    FILE *image_ptr = NULL;

    // Initialize found images counter
    int i = 0;

    // Read BLOCK_SIZE number of bytes from the raw data file, one block at a time
    while (fread(buffer, BLOCK_SIZE, 1, inptr) == 1)
    {
        // Check if a block has image start identifiers
        if (is_image_start(buffer))
        {
            // Close previously opened image file
            if (image_ptr != NULL)
            {
                fclose(image_ptr);
            }

            // Format new image file name
            sprintf(image_name, "%03d.jpg", i);

            // Open new image file, exit on error
            image_ptr = fopen(image_name, "w");
            if (image_ptr == NULL)
            {
                fprintf(stderr, "Failed to open file %s, errno %d\n", image_name, errno);
                fclose(image_ptr);
                fclose(inptr);
                free(buffer);
                return 3;
            }
            i ++;
        }

        // Write into a file, if a pointer is initialized, meaning a file is opened
        if (image_ptr != NULL)
        {
            if (fwrite(buffer, BLOCK_SIZE, 1, image_ptr) != 1)
            {
                fprintf(stderr, "Failed to write into file %s, errno %d\n", image_name, errno);
                fclose(image_ptr);
                fclose(inptr);
                free(buffer);
                return 4;
            }
        }
    }

    // Close last opened image file
    fclose(image_ptr);

    // Close raw data file
    fclose(inptr);

    // Free allocated memory
    free(buffer);

    return 0;
}

int is_image_start(unsigned char *buffer)
{
    return buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0);
}