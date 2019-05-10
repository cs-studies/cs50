from sys import argv
import string
import crypt
from itertools import product

# Define global variables
salt_size = 2
pwd_max_size = 5
pwd_min_size = 1
alphabet = string.ascii_letters


def main():
    validate_input()
    hashed = argv[1]
    salt = hashed[:salt_size]
    for n in range(pwd_min_size, pwd_max_size + 1):
        for xs in product(alphabet, repeat=n):
            pwd = ''.join(xs)
            # Compare if hashed generated password matches a given hash
            if crypt.crypt(pwd, salt) == hashed:
                print(pwd)
                exit(0)
    exit(0)


# Validates user input
def validate_input():
    if len(argv) != 2:
        print("Usage: python crack.py hash")
        exit(1)


if __name__ == "__main__":
    main()