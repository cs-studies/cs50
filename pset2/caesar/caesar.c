#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

string encipher_caesar(string, int);

int main(int argc, string argv[])
{
    // Check arguments
    if (argc != 2)
    {
        printf("Usage: ./caesar k\n");
        return 1;
    }
    int key = atoi(argv[1]);

    // Take user input text
    string text = get_string("plaintext: ");

    // Output the ciphered text
    printf("ciphertext: %s\n", encipher_caesar(text, key));

    return 0;
}

string encipher_caesar(string text, int key)
{
    // For shifting to/from zero starting position
    int shift;
    for (int i = 0, n = strlen(text); i < n; i ++)
    {
        if (isalpha(text[i]))
        {
            shift = isupper(text[i]) ? 'A' : 'a';
            text[i] = (text[i] - shift + key) % 26 + shift;
        }
    }
    return text;
}