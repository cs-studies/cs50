// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;

void free_r(node *node);
unsigned int get_key(char character);

unsigned int trie_size = 0;

unsigned int get_key(char character)
{
    unsigned int key = (character == '\'') ? (N - 1) : (tolower(character) - 'a');
    if (key >= N)
    {
        fprintf(stderr, "Out of bounds key %d for character %c\n", key, character);
        exit(1);
    }
    return key;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
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
    unsigned int key = 0;

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        node *cursor = root;

        // Iterate over each character in a word
        for (int i = 0, word_len = strlen(word); i < word_len; i++)
        {
            key = get_key(word[i]);

            // If a node is not found, create new node
            if (cursor->children[key] == NULL)
            {
                // Allocate memory for the new node
                node *new_node = malloc(sizeof(node));
                if (new_node == NULL)
                {
                    return false;
                }
                // Set node variables initial values
                new_node->is_word = false;
                for (int j = 0; j < N; j++)
                {
                    new_node->children[j] = NULL;
                }
                // Add new node to the trie
                cursor->children[key] = new_node;
            }
            // Move cursor
            cursor = cursor->children[key];
        }

        // Mark the word
        cursor->is_word = true;

        // Increase trie size value
        trie_size ++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return trie_size;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *cursor = root;
    unsigned int key;
    for (int i = 0, word_len = strlen(word); i < word_len; i++)
    {
        key = get_key(word[i]);
        if (!(cursor = cursor->children[key]))
        {
            return false;
        }
    }
    return cursor->is_word;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    free_r(root);
    return size() == 0;
}

void free_r(node *node)
{
    for (int i = 0; i < N; i ++)
    {
        if (node->children[i] != NULL)
        {
            free_r(node->children[i]);
            node->children[i] = NULL;
        }
    }
    if (node->is_word)
    {
        trie_size --;
    }
    node->is_word = NULL;
    free(node);
}
