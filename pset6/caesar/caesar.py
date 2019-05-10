from sys import argv
from cs50 import get_string


def main():
    validate_argv()
    text = prompt_text()
    key = int(argv[1])
    print("ciphertext:", encrypt_caesar(text, key))


# Validates command correctness
def validate_argv():
    if len(argv) != 2:
        print("Usage: python caesar.py k")
        exit(1)


# Prompt user for plain text
def prompt_text():
    return get_string("plaintext: ")


def encrypt_caesar(text, key):
    encrypted = ""
    for c in text:
        # Modify only letters
        if c.isalpha():
            # Preserve case
            shift = ord('A') if c.isupper() else ord('a')
            # Calculate new position of a character
            new_position = (ord(c) - shift + key) % 26
            encrypted += chr(new_position + shift)
        else:
            encrypted += c
    return encrypted


if __name__ == "__main__":
    main()