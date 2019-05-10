// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26*2

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Store found misspelled words to reduce number of searches in the full dictionary
node *misspelled[N];

// Limit number of linked list elements in misspelled buckets for better performance
#define CACHE_ELEMENTS 20

//* *********************************** * //
// *****  MOVE TO DICTIONARY.H ********///
// ** try cache + doubled key ** //
//* *********************************** * //
void init(node *h_table[]);
bool put(const char *word, node *h_table[]);
bool lookup(const char *word, node *h_table[]);
bool free_buckets(node *h_table[]);
unsigned int calculate_size(node *h_table[]);
unsigned int calculate_bucket_size(node *h_table[], int index);

// Hashes word to a number between 0 and 25, inclusive,
// Hash based on a word's first letter and odd/even length
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a' + (strlen(word) % 2 == 0 ? 26 : 0);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    init(hashtable);

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // Calculate hash of a word, check it's less then hashtable size
        if (hash(word) >= N)
        {
            fprintf(stderr, "Hashtable size and calculated hash mismatch\n");
            unload();
            return false;
        }
        put(word, hashtable);
    }

    // Close dictionary
    fclose(file);

    init(misspelled);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return calculate_size(hashtable);
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    if (lookup(word, misspelled))
    {
        // printf("Found in cache %d %s\n", hash(word), word);
        return false;
    }

    if (lookup(word, hashtable))
    {
        return true;
    }

    // Limit cache table size
    if (calculate_bucket_size(misspelled, hash(word)) < CACHE_ELEMENTS)
    {
        // Put the word into misspelled words hash table
        put(word, misspelled);
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    free_buckets(hashtable);
    printf("Cache size: %d\n", calculate_size(misspelled));
    free_buckets(misspelled);
    return calculate_size(hashtable) == 0 && calculate_size(misspelled) == 0;
}

void init(node *h_table[])
{
    // Initialize a hash table
    for (int i = 0; i < N; i++)
    {
        h_table[i] = NULL;
    }
}

bool put(const char *word, node *h_table[])
{
    // Allocate memory for the new node
    node *new_node = malloc(sizeof(node));
    if (new_node == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for a new node\n");
        return false;
    }

    int word_hash = hash(word);

    // Insert a new node into the bucket
    strcpy(new_node->word, word);
    new_node->next = h_table[word_hash] != NULL ? h_table[word_hash] : NULL;

    // Make newly inserted node a bucket head
    h_table[word_hash] = new_node;

    return true;
}

bool lookup(const char *word, node *h_table[])
{
    node *cursor;

    int word_hash = hash(word);

    // Iterate over each bucket of a hashtable
    for (int i = 0; i < N; i ++)
    {
        cursor = h_table[word_hash];

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

bool free_buckets(node *h_table[])
{
    node *node_to_free;
    node *cursor;

    // Iterate over each hashtable element
    for (int i = 0; i < N; i ++)
    {
        // Take pointer to the current bucket head
        cursor = h_table[i];

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
        h_table[i] = NULL;
    }
    return true;
}

unsigned int calculate_size(node *h_table[])
{
    int size = 0;
    node *cursor;

    // Iterate over each hashtable element
    for (int i = 0; i < N; i ++)
    {
        size += calculate_bucket_size(h_table, i);
    }
    return size;
}

unsigned int calculate_bucket_size(node *h_table[], int index)
{
    int size = 0;
    node *cursor;

    // Take pointer to the current bucket head
    cursor = h_table[index];

    // Add each linked list element to the size
    while (cursor != NULL)
    {
        size ++;
        cursor = cursor->next;
    }
    return size;
}