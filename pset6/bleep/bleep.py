from cs50 import get_string
from sys import argv, exit


def main():
    validate_input()
    print(censor(take_input(), read_banned_words()))


def validate_input():
    # Only accept command with one argument
    if len(argv) != 2:
        print("Usage: python bleep.py banned.txt")
        exit(1)


# Returns a set with banned words read from a given file
def read_banned_words():
    banned_file = argv[1]
    banned_words = set()
    # Open file with banned words
    file = open(banned_file, "r")
    if not file:
        print(f"Could not open {banned_file}.")
        exit(1)
    for line in file:
        banned_words.add(line.rstrip("\n"))
    file.close()
    return banned_words


# Prompt user for the text to censor
def take_input():
    return get_string("What message would you like to censor?\n")


# Returns text, in which banned words are replaced with asterisks
def censor(text, banned_words):
    text = text.split()
    censored_text = []
    # Replace each banned words with asterisks
    for word in text:
        censored_text.append('*' * len(word) if word.lower() in banned_words else word)
    return ' '.join(censored_text)


if __name__ == "__main__":
    main()
