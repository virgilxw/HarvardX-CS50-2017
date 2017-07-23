#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int n;
    do
    {
        printf("Height: ");
        n = get_int();
        if (n == 0)
        {
            return 0;
        }
    }while (n < 1 || n > 23);
    
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            printf(" ");
        }
        for (int j = n - 1 - i; j < n; j++)
        {
            printf("#");
        }
        
        printf("  ");
        
        for (int j = n - 1 - i; j < n; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}