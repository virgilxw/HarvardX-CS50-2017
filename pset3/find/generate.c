/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    // Make sure there are only two or three command line arguments
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // Convert the command line arguments into integers
    int n = atoi(argv[1]);

    // seed srand48 with argv[2] if argv[2] is present
    if (argc == 3)
    {
        srand48((long) atoi(argv[2]));
    }
    // if no seed provided, random seed provided
    else
    {
        srand48((long) time(NULL));
    }

    // Generate the number of numbers requested
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
