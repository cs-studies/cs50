#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Take user input
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 0 || height > 23);

    int hashesIndex = height - 1;

    // Draw each row
    while (hashesIndex >= 0)
    {
        // Draw left half-pyramid part
        for (int column = 0; column < height; column ++)
        {
            if (column < hashesIndex)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }

        // Draw central gap
        printf("  ");

        // Draw right half-pyramid part
        for (int column = 0; column < height; column ++)
        {
            if (column < (height - hashesIndex))
            {
                printf("#");
            }
        }

        printf("\n");
        hashesIndex --;
    }
}