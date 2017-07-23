/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"
#include <string.h>

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    //base case
    if (n==1)
    {
        //eprintf("set size 1. value: %i\n", values[0]);
        if(values[0] == value)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    int mid = n/2;
    int newValues[mid];
    if (values[mid] == value)
    {
        //eprintf("mid value = value\n");
        return true;
    }
    else if (values[mid] > value)
    {
        //eprintf("value(%i) < mid(%i)\n",value ,values[mid]);
        for (int i=0; i<mid; i++)
        {
            newValues[i] = values[i];
        }
        return search(value, newValues, mid);
    }
    else if (values[mid] < value)
    {
        //eprintf("value(%i) > mid(%i)\n",value, values[mid]);
        for (int i=0, j=mid+1; j<n; i++)
        {
            newValues[i] = values[j++];
        }
        return search(value, newValues, n-mid-1);
    }
    
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int bound = 65536;
    //counting sort
    //initialising vaalues
    int sortingArray[bound];
    int output[n];
    memset(sortingArray, 0, sizeof(sortingArray));
    
    //counting values
    for (int i=0; i<n; i++)
    {
        sortingArray[values[i]]++;
    }
    
    //Sigmaing values to find position
    for (int i=1; i<bound; i++)
    {
        /**if (sortingArray[i] != 0)
        {
            eprintf("%i:%i\n", i, sortingArray[i]);
        }**/
        sortingArray[i] += sortingArray[i-1];
    }
    
    //placing values
    for (int i=0; i<n; i++)
    {
        output[sortingArray[values[i]]-1] = values[i];
        sortingArray[values[i]]--;
    }
    
    //replacing original
    for (int i=0; i<n; i++)
    {
        //eprintf("output[%i]: %i\n", i, output[i]);
        values[i] = output[i];
    }
}
