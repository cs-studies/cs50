from sys import argv
import string
import crypt

# Define global variables
salt_size = 2
pwd_max_size = 5
pwd_min_size = 1
alphabet = string.ascii_letters


def main():
    validate_input()
    hashed = argv[1]
    salt = hashed[:salt_size]
    # Iterate over each password size
    for size in range(pwd_min_size, pwd_max_size + 1):
        # Generate passwords of the given size
        for pwd in generate_pwd(size):
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


# Passwords generator
def generate_pwd(pos):
    for c in alphabet:
        if pos == 1:
            yield c
        else:
            for p in generate_pwd(pos - 1):
                yield c+p


if __name__ == "__main__":
    main()