// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Declare a variable to store word hash
    int word_hash;

    node *new_node;

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // Calculate hash of a word, check it's less then hashtable size
        word_hash = hash(word);
        if (word_hash >= N)
        {
            fprintf(stderr, "Hashtable size and calculated hash mismatch\n");
            unload();
            return false;
        }

        new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for a new node\n");
            return false;
        }
        // Insert a new node into the bucket
        strcpy(new_node->word, word);
        new_node->next = hashtable[word_hash] != NULL ? hashtable[word_hash] : NULL;

        // Make newly inserted node a bucket head
        hashtable[word_hash] = new_node;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    int size = 0;
    node *cursor;

    // Iterate over each hashtable element
    for (int i = 0; i < N; i ++)
    {
        // Take pointer to the current bucket head
        cursor = hashtable[i];

        // Add each linked list element from a bucket to the size
        while (cursor != NULL)
        {
            size ++;
            cursor = cursor->next;
        }
    }

    return size;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *cursor;

    int word_hash = hash(word);

    // Iterate over each bucket of a hashtable
    for (int i = 0; i < N; i ++)
    {
        cursor = hashtable[word_hash];

        // Iterate over each linked list of a bucket
        while (cursor != NULL)
        {
            // The word is found in a linked list
            if (strcasecmp(cursor->word, word) == 0)
            {
                // The word is found in the dictionary
                return true;
            }
            cursor = cursor->next;
        }
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *node_to_free;
    node *cursor;

    // Iterate over each hashtable element
    for (int i = 0; i < N; i ++)
    {
        // Take pointer to the current bucket head
        cursor = hashtable[i];

        // Add each linked list element from a bucket to the size
        while (cursor != NULL)
        {
            // Store current linked list element
            node_to_free = cursor;

            // Move to the next linked list element
            cursor = cursor->next;

            // Remove previous linked list element
            free(node_to_free);
        }

        hashtable[i] = NULL;
    }

    return size() == 0;
}