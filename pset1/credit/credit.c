#import <cs50.h>
#import <stdio.h>
#import <math.h>

int count_digits(long long);
bool is_luhn_valid(long long);
string identify_card_type(long long, int);

int main(void)
{
    long long card_number;
    // Take use input
    do
    {
        card_number = get_long_long("Enter credit card number: ");
    }
    while (card_number <= 0);

    int count = count_digits(card_number);

    // Identify card type if the given number is valid
    if (count < 13 || count > 16 || !is_luhn_valid(card_number))
    {
        printf("INVALID\n");
    }
    else
    {
        printf("%s\n", identify_card_type(card_number, count));
    }
    return 0;
}

int count_digits(long long number)
{
    int count = 0;
    do
    {
        ++ count;
    }
    while (number /= 10);

    return count;
}

bool is_luhn_valid(long long number)
{
    int total = 0;
    int digit;
    bool do_multiply = false;
    // Calculate Luhn algorithm checksum
    do
    {
        digit = number % 10;
        if (do_multiply)
        {
            digit *= 2;
            if (digit > 9)
            {
                // The same as to sum up two digits this number consists of
                digit -= 9;
            }
        }
        total += digit;
        do_multiply = !do_multiply;
    }
    while (number /= 10);
    // Checksum has to end with zero for a valid number
    return total % 10 == 0;
}

string identify_card_type(long long number, int count)
{
    string card_type = "INVALID";

    // Find first digit(s)
    long long first_two_digits = number;
    int first_digit;
    for (int i = 0; i < count - 2; i ++)
    {
        first_two_digits /= 10;
    }
    first_two_digits = round(first_two_digits);
    first_digit = round(first_two_digits / 10);

    // Identify card type by the first digit(s)
    if (first_digit == 4)
    {
        card_type = "VISA";
    }
    else if (first_two_digits == 34 || first_two_digits == 37)
    {
        card_type = "AMEX";
    }
    else
    {
        int mastercard[] = {51, 52, 53, 54, 55};
        for (int i = 0; i < 5; i++)
        {
            if (mastercard[i] == first_two_digits)
            {
                card_type = "MASTERCARD";
            }
        }
    }
    return card_type;
}