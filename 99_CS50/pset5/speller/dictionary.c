
// dictionary.c
//------------------------------------------
// implements a dictionary's functionality
//------------------------------------------

#include <stdbool.h>

#include "dictionary.h"

#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Represents a node in a hash table.
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table.
const unsigned int N = 35000;

// Hash table
node *table[N];

//---------------------------------------

// Returns true if word is in dictionary else false.
bool check(const char *word)
{
    //create a copy of the word.
    char temp[strlen(word)];

    //copy word to temp array.
    strcpy(temp, word);

    //hash the word in the temp array.
    int hash_index = hash(temp);

    //set cursor to index of word in temp array.
    node *cursor = table[hash_index];

    //compare word in temp array with words in hash table.
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, temp) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

//---------------------------------------

// Hashes word to a number
unsigned int hash(const char *word)
{
    //hash function "djb2" first reported by "dan bernstein". (sourced from http://www.cse.yorku.ca/~oz/hash.html).
    //modified to drop all letters to lower case.
    unsigned long hash = 5381;

    int c;

    //iterate accross word array.
    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + tolower(c);
    }
    return hash % N;
}

//---------------------------------------

// Loads dictionary into memory, returning true if successful else false.
bool load(const char *dictionary)
{
    //opens dictionary file
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open dictionary 1");
        return false;
    }

    //declare word array to act as buffer.
    char dict_word[LENGTH + 1];

    //reads strings one at a time into an array called "word".
    while (fscanf(dict, "%s", dict_word) != EOF)
    {
        //creates a new node for each word.
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            printf("Could not allocate memory");
            return false;
        }

        //copy word into node.
        strcpy(n->word, dict_word);
        n->next = NULL;

        //hash word to obtain hash value.
        int hash_value = hash(dict_word);

        //insert node into hash table. being sure not to 'lose' the rest of the linked list.
        if (table[hash_value] == NULL)
        {
            table[hash_value] = n;
            n->next = NULL;
        }
        else
        {
            n->next = table[hash_value];
            table[hash_value] = n;
        }
    }
    fclose(dict); // forgetting to close the dictionary resulted in memory issues with check50.
    return true;
}

//---------------------------------------

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    int word_count = 0;

    //iterate over all the buckets.
    for (int i = 0; i < N; i++)
    {
        node *array = table[i];

        //iterate over each word in the bucket.
        while (array != NULL)
        {
            word_count++;
            array = array->next;
        }
    }
    return word_count;
}

//---------------------------------------

// Unloads dictionary from memory, returning true if successful else false.
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *tmp = NULL;

        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}