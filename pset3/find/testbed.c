#include <stdio.h>
#include <cs50.h>

int main (void)
{
    int a[] = {1,2,3};
    int n = 3;
    int mid = n/2;
    int b[] = a[0:mid];
    for (int i=0; i<mid;i++)
    {
        printf("%i\n",b[i]);
    }
}