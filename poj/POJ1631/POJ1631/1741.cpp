// POJ1631.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define POJ1741_
#ifdef POJ1741

#include <stdio.h>
#include <math.h>
long long myPow(int x, int y)
{
    long long value = 1;
    for (int i = 0; i < y; ++i)
    {
        value *= x;
    }

    return value;
}

int getResult(long long value)
{
    int flag = 1;

    if (value < 0)
    {
        value = -value;
        flag = -1;
    }
    int maxBase = sqrt(value*1.0) + 0.1;
    //int maxPow = log2(value*1.0) + 0.1;
    int maxPow = log(value*1.0) / log(2.0) + 0.1;
    int result = 1;

    if (maxBase < 2)
    {
        maxBase = 2;
    }

    for (int base = 2; base <= maxBase; ++base)
    {
        for (int po = 1; po <= maxPow; ++po)
        {
            long long tmpResult = myPow(base, po);
            if (tmpResult == value)
            {
                //rseult = po;
                if (po % 2 || flag == 1)
                {
                    return po;
                }
            }

            if (tmpResult > value)
            {
                break;
            }
        }
    }

    return result;

}

int main()
{
    long long value;

    while (1)
    {
        scanf("%lld", &value);
        if (value == 0)
        {
            break;
        }

        printf("%d\n", getResult(value));
    }
}


#endif
