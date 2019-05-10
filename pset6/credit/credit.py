from cs50 import get_int


def main():
    number = get_number()
    if not is_valid(number):
        print("INVALID")
    else:
        print(get_card_type(number))


# Prompts user for card number
def get_number():
    while True:
        number = get_int("Number: ")
        if number >= 0:
            # Accept only positive numbers
            return number


# Validates credit card number by size and using Luhn algorithm
def is_valid(number):
    number_len = len(str(number))
    return number_len >= 13 and number_len <= 16 and is_luhn_valid(number)


# Implementation of Luhn algorithm
def is_luhn_valid(number):
    numbers = str(number)[::-1]
    sum = 0
    for i, num in enumerate(numbers, start=1):
        number = int(num) * 2 if i % 2 == 0 else int(num)
        sum += number if number <= 9 else (number % 10) + 1
    return sum % 10 == 0


# Identifies card type by the first digits
# Supports Visa, Mastercard, and Amex cards
def get_card_type(number):
    first_two_digits = str(number)[:2]
    if first_two_digits[:1] == '4':
        return "VISA"
    elif first_two_digits == '34' or first_two_digits == '37':
        return "AMEX"
    else:
        first_two_digits = int(first_two_digits)
        if first_two_digits >= 51 and first_two_digits <= 55:
            return "MASTERCARD"
    return "INVALID"


if __name__ == "__main__":
    main()
