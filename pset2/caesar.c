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
    int key = atoi(argv[1]);
    
    printf("plaintext: ");
    string plainText = get_string();
    string cypherText = plainText;
    
    for (int i=0, n=strlen(plainText); i<n; i++)
    {
        if (isalpha(plainText[i]))
        {
            cypherText[i] = plainText[i] + key;
            while(isalpha(cypherText[i]) == false)
            {
                cypherText[i] -= alphabetLength;
            }
        }
    }
    
    printf("ciphertext: %s\n", cypherText);
}