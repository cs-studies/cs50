#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_valid_keyword(string);
string encipher_vigenere(string, string);

int main(int argc, string argv[])
{
    if (argc != 2 || !is_valid_keyword(argv[1]))
    {
        printf("Usage: ./vigenere k\n");
        return 1;
    }

    string keyword = argv[1];

    string text = get_string("plaintext: ");

    printf("ciphertext: %s\n", encipher_vigenere(text, keyword));

    return 0;
}

int is_valid_keyword(string keyword)
{
    for (int i = 0, n = strlen(keyword); i < n; i ++)
    {
        if (!isalpha(keyword[i]))
        {
            return 0;
        }
    }
    return 1;
}

string encipher_vigenere(string text, string keyword)
{
    char key;
    int shift, keyword_len = strlen(keyword);

    for (int i = 0, n = strlen(text), j = 0; i < n; i ++)
    {
        if (isalpha(text[i]))
        {
            // Set shifting value
            shift = isupper(text[i]) ? 'A' : 'a';
            // Find a key to use
            key = isupper(keyword[j]) ? keyword[j] - 'A' : keyword[j] - 'a';

            // Encipher, using key and shifting value
            text[i] = (text[i] - shift + key) % 26 + shift;

            // Set keyword character index to use for the next text character
            j ++;
            if (j == keyword_len)
            {
                j = 0;
            }
        }
    }
    return text;
}