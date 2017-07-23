/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

int no_words = 0;

//trie struct
typedef struct lib_trie_node
{
    bool is_word;
    struct lib_trie_node *children[27];
}lib_trie_node;

//default trie root
lib_trie_node LIB_TRIE_ROOT;

//adds word to trie starting at LIB_TRIE_ROOT
bool lib_trie_add(const char *word, lib_trie_node *root)
{
    //initialise variables
    int c;
    
    //debug
    //printf("adding word \"%s\", character \"%c\".\n", word, word[0]);
    
    //base case
    if (word[0] == '\0')
    {
        root->is_word = true;
        
        //debug
        //printf("parsed base case \\n.\n");
        
        return true;
    }
    
    //convert characters to value
    if (word[0] == '\'')
        c = 0;
    else
        c = word[0] - 96;
    
    //create trie node if it does not exist
    if (root->children[c] == NULL)
    {
        root->children[c] = (struct lib_trie_node *)malloc(224);
    }
    
    //recursion
    return lib_trie_add (&word[1], root -> children[c]);
}

void lib_trie_unload(lib_trie_node *root)
{
    //iterate through the children, unloading them before freeing them in turn
    for (int i = 0; i < 27; i++)
    {
        if (root -> children[i] != NULL)
        {
            lib_trie_unload(root -> children[i]);
        }
    }
    //once all nodes are freed, free the root
    free(root);
}


bool lib_trie_check(const char *word, lib_trie_node root)
{
    //initialise variables
    int c;
    
    //base case
    if (word[0] == '\0')
    {
        return root.is_word;
    }
    
    //convert characters to value
    if (word[0] == '\'')
        c = 0;
    else
        c = tolower(word[0]) - 96;
    
    //create trie node if it does not exist
    if (root.children[c] == NULL)
    {
        return false;
    }
    
    //recursion
    return lib_trie_check (&word[1], *root.children[c]);
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    //debug
    //printf("checking word %s\n", word);
    
    return lib_trie_check(word, LIB_TRIE_ROOT);
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    //read file
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("Could not open %s in load().\n", dictionary);
        unload();
        return 1;
    }
    
    //iterate through the dictionary
    int i = 0;
    char word[LENGTH+1];
    no_words = 0;
    
    for (char c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        //if not newline charcter, add it to word[i] array
        if (c != '\n')
        {
            word[i] = c;
            i++;
        }
        
        //if newline, parse new word into trie
        else
        {
            //terminate current word
            word[i]= '\0';
            no_words++;
            //testing -DELETE AFTER-
            //bool add_to_trie_success = true;
            //printf("Adding word %s to trie.\n", word);
            
            //input word into trie
            bool add_to_trie_success = lib_trie_add(word, &LIB_TRIE_ROOT);
            if (!add_to_trie_success)
            {
                printf("Could not load word !%s.\n", word);
                unload();
                return false;
            }
            
            //reset index
            i = 0;
        }
    }
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return no_words;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    for (int i = 0; i < 27; i++)
    {
        if (LIB_TRIE_ROOT.children[i] != NULL)
        {
            lib_trie_unload(LIB_TRIE_ROOT.children[i]);
        }
    }
    return true;
}