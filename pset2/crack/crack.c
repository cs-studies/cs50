#define _XOPEN_SOURCE
#include <unistd.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

#define PWD_MAX_SIZE 5
#define SALT_SIZE 2
#define VOCABULARY_SIZE 52

char vocabulary[VOCABULARY_SIZE];

void set_vocabulary();
int crack_pwd(string, int, int, char[], string);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    string hash = argv[1];

    // Allocate memory to hold salt size and null terminator
    char salt[SALT_SIZE + 1];
    strncat(salt, hash, SALT_SIZE);

    // Passwords will be generated taking chars from the pre-generated vocabulary
    set_vocabulary();

    // Attempt password cracking for each password size
    for (int size = 1; size <= PWD_MAX_SIZE; size ++)
    {
        char pwd[size + 1];
        memset(pwd, 0, size + 1);
        if (crack_pwd(pwd, 0, size, salt, hash))
        {
            break;
        }
    }

    return 0;
}

void set_vocabulary()
{
    int letters_number = 'z' - 'a' + 1;
    // Write lowercase and uppercase letters into one array
    for (int i = 0, j = letters_number; i < letters_number; i ++, j ++)
    {
        vocabulary[i] = 'a' + i;
        vocabulary[j] = 'A' + i;
    }
}

int crack_pwd(char pwd[], int index, int pwd_size, char salt[], string hash)
{
    string generated;
    for (int i = 0; i < VOCABULARY_SIZE; i ++)
    {
        pwd[index] = vocabulary[i];

        // Currently generated password has not reached yet the required size
        if (index < (pwd_size - 1))
        {
            // Set the next letter of the generated password
            if (crack_pwd(pwd, index + 1, pwd_size, salt, hash))
            {
                // 1 is returned only if a matching password has been generated
                return 1;
            }
        }
        else
        {
            // Check if a generated password hash matches a given hash
            generated = crypt(pwd, salt);
            if (strcmp(generated, hash) == 0)
            {
                // Output cracked password
                printf("%s\n", pwd);
                // Output a signal to exit from this function
                return 1;
            }
        }
    }
    // Matching password has not been found
    return 0;
}
