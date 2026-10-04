#include <iostream>
#include <stdio.h>

static void printBinary1(char num)
{
    for (char i = sizeof(char) * 8 - 1; i >= 0; i--)
    {
        std::cout << ((num >> i) & 1);
    }
    printf("\n");
}

static void printBinary1(uint16_t num)
{
    for (char i = sizeof(uint16_t) * 8 - 1; i >= 0; i--)
    {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");
}

int main()
{
    std::cout << "HKAJSFHDJKh" << std::endl;

    //0b11111001
    char test = -7;
    printBinary1(test);
    uint16_t mask = 0b1111111100000000;
    int16_t convert = test | mask;
    std::cout << convert << std::endl;

    return 0;
}