// POJ1631.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define POJ1018_
#ifdef POJ1018
#include <stdio.h>
#include <algorithm>
using namespace std;
const int maxTestCount = 101;
typedef struct
{
    unsigned int bandWidth;
    unsigned int price;
}Device;
bool cmp(const Device a, const Device b)
{
    return (a.bandWidth > b.bandWidth) || (a.bandWidth == b.bandWidth && a.price < b.price);
}
Device device[maxTestCount][maxTestCount] = { 0 };
int deviceCount[maxTestCount] = { 0 };

const int MaxInt = 2147483647;
//char 
int main()
{
    int testCount = 0;
    scanf("%d", &testCount);
    while (testCount--)
    {
        int minBandwidth = 0;
        int maxBandwidth = MaxInt;
        int count = 0;
        scanf("%d", &count);
        for (int i = 0; i<count; ++i)
        {
            int tmpMinBandwidth = MaxInt;
            int tmpMaxBandwidth = 0;
            scanf("%d", deviceCount + i);
            for (int k = 0; k < deviceCount[i]; ++k)
            {
                scanf("%d%d", &device[i][k].bandWidth, &device[i][k].price);
                if (device[i][k].bandWidth > tmpMaxBandwidth)
                {
                    tmpMaxBandwidth = device[i][k].bandWidth;
                }

                if (device[i][k].bandWidth < tmpMinBandwidth)
                {
                    tmpMinBandwidth = device[i][k].bandWidth;
                }
            }

            if (tmpMaxBandwidth < maxBandwidth)
            {
                maxBandwidth = tmpMaxBandwidth;
            }

            if (tmpMinBandwidth > minBandwidth)
            {
                minBandwidth = tmpMinBandwidth;
            }

            //sort the device by
            sort(device[i], device[i] + deviceCount[i], cmp);
        }

        double ratio = 0;
        for (int i = 0; i < count; ++i)
        {
            for (int k = 0; k < deviceCount[i]; ++k)
            {
                int deviceMinBandWidth = device[i][k].bandWidth;
                //if (deviceMinBandWidth < minBandwidth || deviceMinBandWidth > maxBandwidth)
                //{
                //    continue;
                //}

                if (deviceMinBandWidth > maxBandwidth)
                {
                    continue;
                }

                int price = device[i][k].price;
                int bandwidth = device[i][k].bandWidth;
                for (int i1 = 0; i1 < count; ++i1)
                {
                    if (i1 == i)
                    {
                        continue;
                    }

                    double minP = MaxInt;
                    double tmpP = MaxInt;
                    for (int k1 = 0; k1 < deviceCount[i1]; ++k1)
                    {
                        if (device[i1][k1].bandWidth >= bandwidth)
                        {
                            //price += device[i1][k1].price;
                            if (tmpP > device[i1][k1].price)
                            {
                                tmpP = device[i1][k1].price;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }

                    price += tmpP;
                }

                double newRatio = bandwidth*1.0 / price;
                if (newRatio > ratio)
                {
                    ratio = newRatio;
                }
            }
        }

        printf("%.3lf\n", ratio);
    }

    return 0;
}

#endif
