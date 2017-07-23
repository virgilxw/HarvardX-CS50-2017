#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

#define alphabetLength 26

int main (int argc,string argv[])
{
    
    if (argc == 1)
    {
        printf("No CLI detected, exiting.\n");
        return 1;
    }
    else if(argc != 2)
    {
        printf("More than two CLI detected, exiting.\n");
        return 1;
    }
    
    string key = argv[1];
    
    for (int i = 0, n = strlen(key); i<n; i++)
    {
        if (!isalpha(key[i]))
        {
            printf("key is non-alpha, exiting.\n");
            return 1;
        }
        key[i] = tolower(key[i]);
    }
    
    printf("plaintext: ");
    string plainText = get_string();
    string cypherText = plainText;
    
    for (int i=0, n = strlen(plainText), keyIndex = 0, keyLength = strlen(key); i<n; i++)
    {
        if (islower(plainText[i]))
        {
            cypherText[i] = ((plainText[i] - 96 + key[keyIndex] - 97) % alphabetLength) + 96;
            keyIndex++;
        }
        if (isupper(plainText[i]))
        {
            cypherText[i] = ((plainText[i] - 64 + key[keyIndex] - 97) % alphabetLength) + 64;
            keyIndex++;
        }
        if (keyIndex == keyLength)
            {
                keyIndex = 0;
            }
    }
    
    printf("ciphertext: %s\n", cypherText);
}