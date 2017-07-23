#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <crypt.h>
#include <cs50.h>
#include <string.h>

#define seedSetLength 64

#define pwchars "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
#define seedchars "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.0123456789/"

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
    
    printf("testing for hash, %s\n", argv[1]);
    string password = "rofl";
    string salt = "50";
    string generatedHash = malloc(13);
    
    
    printf("Testing password %s, salt %s\n", password, salt);
    generatedHash = crypt(password, salt);
    printf("generated hash: %s\n", generatedHash);
    printf("%s\n", argv[1]);
    if (strcmp(generatedHash, argv[1]) == 0)
    {
         printf("\n-----\nFound! Hash %s:\n Password: %s, salt: %s\n-----\n", argv[1], password, salt);
        return 0;
    }
}