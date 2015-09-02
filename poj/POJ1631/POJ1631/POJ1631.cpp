// POJ1631.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#ifdef POJ1631

#include <stdio.h>
const int maxSize = 40001;
int data[maxSize] = { 0 };
int minVal[maxSize] = { 0 };
int main()
{
    int count;
    scanf("%d", &count);
    while (count--)
    {
        int itemCount;
        scanf("%d", &itemCount);

        for (int i = 0; i < itemCount; ++i)
        {
            scanf("%d", i+data);
        }

        minVal[0] = 0;
        minVal[1] = data[0];
        int start = 1;
        int end = 1;
        for (int i = 1; i < itemCount; ++i)
        {
            int high = end;
            int low = start;
            int t = end;
            int value = data[i];

            if (value > minVal[end])
            {
                minVal[++end] = value;
                continue;
            }

            while (1)
            {
                int dec = 0;
                int t = (low + high) / 2;
                if (value > minVal[t])
                {
                    dec = 0;
                    low = t;
                }
                else if (value < minVal[t])
                {
                    high = t;
                    dec = 1;
                }
                else if (value == minVal[t])
                {
                    break;
                }

                if (low == high)
                {
                    minVal[low] = value;
                    break;
                }

                if (low + 1 >= high)
                {
                    if (minVal[low] == value || minVal[high] == value)
                    {
                        break;
                    }

                    for (int x = low; x <= high; ++x)
                    {
                        if (minVal[x] >= value)
                        {
                            minVal[x] = value;
                            break;
                        }
                    }

                    break;
                }
            }
        }

        printf("%d\n", end);
    }
	return 0;
}


#endif
