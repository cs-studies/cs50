from sys import argv
from cs50 import get_string


def main():
    # Validate command
    validate_argv()
    # Prompt user for plain text
    plaintext = take_input()
    # Encrypt plain text using a given key
    encrypted = encrypt_vigenere(plaintext, argv[1])
    print("ciphertext:", encrypted)


def validate_argv():
    if len(argv) != 2 or not argv[1].isalpha():
        print("Usage: python vigenere.py k")
        exit(1)


def take_input():
    while True:
        text = get_string("plaintext: ")
        if text:
            return text


def encrypt_vigenere(text, key):
    encrypted = ""
    key_len = len(key)
    key_index = 0
    for c in text:
        if not c.isalpha():
            encrypted += c
        else:
            # Calculate key position
            key_position = ord(key[key_index]) - calculate_shift(key[key_index])
            # Calculate character shift
            shift = calculate_shift(c)
            # Calculate encrypted character position
            new_position = (ord(c) - shift + key_position) % 26 + shift
            encrypted += chr(new_position)
            # Calculate key_index for the next iteration
            key_index = (key_index + 1) % key_len
    return encrypted


def calculate_shift(c):
    return ord('a' if c.islower() else 'A')


if __name__ == "__main__":
    main()