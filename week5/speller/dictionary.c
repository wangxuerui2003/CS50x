// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
// Use of the NULL identifier
#include <stdlib.h>
// Use of strcasecmp
#include <strings.h>
// Use of fopen, fsanf...
#include <stdio.h>
// Use of strcpy
#include <string.h>

#include "dictionary.h"

//define the size of hash table
#define TABLE_SIZE 10000
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Structure of a hash table
typedef struct
{
    int     size;
    node    *table[TABLE_SIZE];
}
t_table;

// Init the dictionary table as a global variable and set to NULL as unloaded
t_table table;

// Free linked list function
void    ft_free_list(node *head)
{
    if (head->next == NULL)
    {
        free(head);
        return ;
    }
    ft_free_list(head->next);
    free(head);
}

// Function to add word to the hash table while loading the dictionary
bool    add_word(const char *word)
{
    int     index;
    node    *temp;
    int     i;

    index = hash(word);
    temp = table.table[index];
    if (temp == NULL)
    {
        table.table[index] = malloc(sizeof(node));
        if (table.table[index] == NULL)
        {
            return (false);
        }
        strcpy(table.table[index]->word, word);
        table.table[index]->next = NULL;
        return (true);
    }
    i = 0;
    while (temp->next != NULL)
    {
        temp = temp->next;
        i++;
    }
    temp->next = malloc(sizeof(node));
    if (temp->next == NULL)
    {
        return (false);
    }
    strcpy(temp->next->word, word);
    temp->next->next = NULL;
    return (true);
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node    *temp;

    temp = table.table[hash(word)];
    while (temp != NULL)
    {
        if (strlen(word) == strlen(temp->word) && strcasecmp(temp->word, word) == 0)
        {
            return (true);
        }
        temp = temp->next;
    }
    return (false);
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // hash according to the sum of the ascii value of all letters in a word
    int sum;

    if (word == NULL)
    {
        return (0);
    }
    sum = 0;
    for (int i = 0; word[i] != 0; i++)
    {
        if (word[i] >= 65 && word[i] <= 90)
        {
            sum += (word[i] + 32) * (word[i] + 32) + i * i;
        }
        else
        {
            sum += word[i] * word[i] + i * i;
        }
    }
    sum += strlen(word);
    return (sum % TABLE_SIZE);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE    *dict_ptr;
    char    word[LENGTH + 1];

    // Init table to NULL for all node
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        table.table[i] = NULL;
    }
    dict_ptr = fopen(dictionary, "r");
    if (dict_ptr == NULL)
    {
        return (false);
    }
    table.size = 0;
    while (fscanf(dict_ptr, "%s", word) != EOF)
    {
        if (add_word(word) == false)
        {
            return (false);
        }
        table.size++;
    }
    fclose(dict_ptr);
    return (true);
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return (table.size);
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (table.table[i] != NULL)
        {
            ft_free_list(table.table[i]);
            table.table[i] = NULL;
        }
    }
    return (true);
}
