#import <cs50.h>
#import <stdio.h>
#import <math.h>

int main(void)
{
    int coins[] = {25, 10, 5, 1};
    int coins_count = 4;
    int result = 0;

    // Get user input
    float number;
    do
    {
        number = get_float("Change owed: ");
    }
    while (number < 0);

    // Convert to cents
    int cents = round(number * 100);

    // Calculate how many coins to return
    float coins_quantity;
    for (int i = 0; i < coins_count; i ++)
    {
        coins_quantity = cents / coins[i];
        if (coins_quantity > 0)
        {
            cents -= coins[i] * coins_quantity;
            result += coins_quantity;
        }
    }
    printf("%d\n", result);
}