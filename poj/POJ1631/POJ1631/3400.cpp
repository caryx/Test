// POJ1631.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define POJ3400
#ifdef POJ3400

#include <stdio.h>
#include <algorithm>
#include <vector>
using namespace std;


const int maxCount = 10010;
int Value[maxCount] = { 0 };
int Weight[maxCount] = { 0 };
int data[maxCount] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

void swap(int& x, int& y)
{
    int t = x;
    x = y;
    y = t;
}

const int MaxInt = 1073741824;
int result = 0;
int aCost = 1073741824;
int bCost = 0;

int m = 0;
void perm(int total, int n)
{
    if (n == total)
    {
        int aCostTmp = MaxInt;
        int bCostTmp = 0;
        int aWeightTmp = 0;
        int bWeightTmp = 0;
        int nextA = 1;
        for (int i = 0; i<total; ++i)
        {
            if (nextA)
            {
                aWeightTmp += Weight[data[i]];
                aCostTmp += Value[data[i]];

                if (aWeightTmp > bWeightTmp + m)
                {
                    nextA = 0;
                }
            }
            else
            {
                bWeightTmp += Weight[data[i]];
                bCostTmp += Value[data[i]];

                if (bWeightTmp > aWeightTmp + m)
                {
                    nextA = 1;
                }
            }

            //printf("%d ", data[i]);
            //printf("%d ", bCostTmp);

            if (bCostTmp > bCost)
            {
                bCost = bCostTmp;
            }
        }

        //printf("\n");
    }

    for (int i = n; i<total; ++i)
    {
        swap(data[i], data[n]);
        perm(total, n + 1);
        swap(data[n], data[i]);
    }
}
//
//void perm2(int total, int n)
//{
//    if (n == total)
//    {
//
//        for (int i = 0; i<total; ++i)
//        {
//            //aCost
//            printf("%d ", data[i]);
//        }
//
//        printf("\n");
//    }
//
//    for (int i = n; i<total; ++i)
//    {
//        swap(data[i], data[n]);
//        perm(total, n + 1);
//        swap(data[n], data[i]);
//    }
//}

char dataMap[maxCount] = { 0 };

int dfsNextA = 1;
int dfsBValue = 0;
int dfsAValue = MaxInt;
int tmpDfaAValue = 0;
int tmpDfaBValue = 0;
int tmpAWeight = 0;
int tmpBWeight = 0;
void dfs(int total, int depth)
{
    int HasValue = 0;
    for (int i = 0; i < total && depth; ++i)
    {
        if (!dataMap[i])
        {
            HasValue = 1;
            int lastNextA = dfsNextA;

            int lastTmpDfsAValue = tmpDfaAValue;
            int lastTmpDfsBValue = tmpDfaBValue;
            int lastTmpAWeight = tmpAWeight;
            int lastTmpBWeight = tmpBWeight;
            if (dfsNextA)
            {
                tmpDfaAValue += Value[i];
                tmpAWeight += Weight[i];

                if (tmpAWeight > tmpBWeight + m)
                {
                    dfsNextA = 0;
                }
            }
            else
            {
                tmpDfaBValue += Value[i];
                tmpBWeight += Weight[i];

                if (tmpBWeight > tmpAWeight + m)
                {
                    dfsNextA = 1;
                }
            }

            if (tmpAWeight >= dfsAValue)
            {            
                dataMap[i] = 0;
                dfsNextA = lastNextA;
                tmpDfaAValue = lastTmpDfsAValue;
                tmpDfaBValue = lastTmpDfsBValue;
                tmpAWeight = lastTmpAWeight;
                tmpBWeight = lastTmpBWeight;
                continue;
            }
            dataMap[i] = 1;
            dfs(total, depth-1);
            dataMap[i] = 0;
            dfsNextA = lastNextA;
            tmpDfaAValue = lastTmpDfsAValue;
            tmpDfaBValue = lastTmpDfsBValue;
            tmpAWeight = lastTmpAWeight;
            tmpBWeight = lastTmpBWeight;
        }
    }

    if (HasValue == 0)
    {
        if (dfsBValue < tmpDfaBValue)
        {
            dfsBValue = tmpDfaBValue;
            dfsAValue = tmpDfaAValue;
        }
    }
}


void init()
{
    memset(dataMap, 0, sizeof(dataMap));
    dfsNextA = 1;
    dfsBValue = 0;
    dfsAValue = MaxInt;
    tmpDfaAValue = 0;
    tmpDfaBValue = 0;
    tmpAWeight = 0;
    tmpBWeight = 0;
}



//int main()
//{
//    int n = 0;
//    while (~scanf("%d%d", &n, &m))
//    {
//        for (int i = 0; i<n; ++i)
//        {
//            scanf("%d%d", Weight + i, Value + i);
//            data[i] = i;
//        }
//
//        init();
//
//        aCost = MaxInt;
//        bCost = 0;
//        result = 0;
//        //perm(n, 0);
//        //printf("%d\n", bCost);
//        dfs(n, n);
//        printf("%d\n", dfsBValue);// bCost);
//    }
//
//
//    return 0;
//}



#include <stdio.h>
int main()
{
    int count = 0;
    scanf("%d", &count);
    for (int k = 1; k <= count;++k)
    {
        int a;
        scanf("%d", &a);
        int start = 0;
        int end = 0;
        int result = 0;
        int tmpStart = 0;
        int tmpEnd = 0;
        int tmpResult = 0;
        int newSegment = 1;
        for (int i = 0; i < a; ++i)
        {
            if (newSegment)
            {
                newSegment = 0;
                tmpStart = i;
            }

            int value;
            scanf("%d", &value);
            if (tmpResult + value >= 0)
            {
                tmpResult += value;
                tmpEnd = i;
            }
            else
            {
                newSegment = 1;
                tmpResult = 0;
            }

            if (tmpResult > result)
            {
                result = tmpResult;
                end = tmpEnd;
                start = tmpStart;
            }
        }
        printf("Case %d:\n",k);
        printf("%d %d %d\n\n", result,start+1, end+1);
    }
    return 0;
}


//#include <stdio.h>
//#define MAXN 100005
//
//typedef struct{ int s, l, r; }Sum;
//
//Sum MaxSub(int a[], int n);
//
//int main(void)
//{
//    int ar[MAXN], c, n, i, j;
//    Sum x = { 0, 0, 0 };
//    //freopen("input.txt", "r", stdin);
//    //freopen("output.txt", "w", stdout);
//    scanf("%d", &c);
//    for (i = 1; i <= c; i++)
//    {
//        scanf("%d", &n);
//        for (j = 1; j <= n; j++)
//            scanf("%d", &ar[j]);
//        x = MaxSub(ar, n);
//        printf("Case %d:\n%d %d %d", i, x.s, x.l, x.r);
//        if (i != c) printf("\n\n");
//        else printf("\n");
//    }
//}
//
//Sum MaxSub(int a[], int n)
//{
//    int i, s = 0, l = 1, r = 1, b;
//    Sum sum = { a[1], 0, 0 };
//    for (i = 1; i <= n; i++)
//    {
//        if (s >= 0)
//        {
//            s += a[i];
//            if (a[i] > 0) r = i;
//        }
//        else
//        {
//            s = a[i];
//            r = l = i;
//        }
//        if (s >= sum.s)
//        {
//            sum.s = s;
//            sum.l = l;
//            sum.r = r;
//        }
//    }
//    return sum;
//}

#endif

