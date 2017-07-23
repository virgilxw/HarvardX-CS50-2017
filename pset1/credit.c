#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Number: ");
    long long cardNumber = get_long_long();
    long long cardNumberCopy = cardNumber;
    int digit = 0;
    int checksum = 0;
    int firstTwoDigits;
    
    while (cardNumberCopy > 0)
    {
        if (cardNumberCopy < 100 && cardNumberCopy > 9)
        {
            firstTwoDigits = cardNumberCopy;
        }
        int lastdigit = cardNumberCopy % 10;
        digit++;
        
        if (digit % 2 == 0)
        {
            if (lastdigit >= 5)
            {
                checksum += 1 + (lastdigit * 2 - 10);
            }
            else
            {
                checksum += lastdigit * 2;
                }
        }
        else
        {
            checksum += lastdigit;
        }
        cardNumberCopy /= 10;
    }
    
    if (checksum % 10 != 0)
    {
        printf("INVALID\n");
    }
    else if (digit == 13)
    {
        switch (firstTwoDigits/10)
        {
            case 4:
                printf("VISA\n");
                break;
            default:
                printf("INVALID\n");
                break;
        }
    }
    else if (digit == 15)
    {
        switch (firstTwoDigits)
        {
            case 34:
            case 37:
                printf("AMEX\n");
                break;
            default:
                printf("INVALID\n");
                break;
        }
    }
    else if (digit == 16)
    {
        switch (firstTwoDigits)
        {
            case 40:
            case 41:
            case 42:
            case 43:
            case 44:
            case 45:
            case 46:
            case 47:
            case 48:
            case 49:
                printf("VISA\n");
                break;
            case 51:
            case 52:
            case 53:
            case 54:
            case 55:
                printf("MASTERCARD\n");
                break;
            default:
                printf("INVALID\n");
                break;
        }
    }
    return 0;
}