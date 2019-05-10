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

    // Build a half-pyramid
    int hashesStartAt = height - 1;
    while (hashesStartAt >= 0)
    {
        // Draw a row
        for (int column = 0; column <= height; column ++)
        {
            if (column < hashesStartAt)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }
        printf("\n");
        hashesStartAt --;
    }


    // Optional solution looping over rows explicitly
    // for (int row = height; row > 0; row --)
    // {
    //     // Draw columns
    //     for (int column = 0; column <= height; column ++)
    //     {
    //         if (column < hashesStartAt)
    //         {
    //             printf(" ");
    //         }
    //         else
    //         {
    //             printf("#");
    //         }
    //     }
    //     printf("\n");
    //     hashesStartAt --;
    // }
}