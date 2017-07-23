#include <stdio.h>
#include <crypt.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>

#define seedSetLength 64
#define seedchars "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789/."


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
    string password = malloc(4);
    string salt = malloc(2);
    string generatedHash = malloc(13);
    
    //pasword iterator
    for(int pw0=0; pw0<seedSetLength; pw0++)
    {
        password[0] = seedchars[pw0];
        for(int pw1=0; pw1<seedSetLength; pw1++)
        {
            password[1] = seedchars[pw1];
            for(int pw2=0; pw2<seedSetLength; pw2++)
            {
                password[2] = seedchars[pw2];
                for(int pw3=0; pw3<seedSetLength; pw3++)
                {
                    password[3] = seedchars[pw3];
                    
                    //salt iterator
                    for(int salt0=0; salt0<seedSetLength; salt0++)
                    {
                        salt[0] = seedchars[salt0];
                        for(int salt1=0; salt1<seedSetLength; salt1++)
                        {
                            salt[1] = seedchars[salt1];
                            printf("Testing password %s, salt %s\n", password, salt);
                            generatedHash = crypt(password, salt);

                            if (strcmp(generatedHash, argv[1]) == 0)
                            {
                                printf("\n-----\nFound! Hash %s:\n Password: %s, salt: %s\n-----\n", argv[1], password, salt);
                                return 0;
                            }
                            
                        }
                    }
                }
            }
        }
    }
    printf("Not Found\n");
    return -1;
}