// POJ2533.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#ifdef abcx
#include <stdio.h>

int count = 0;
int* data;
int* dataLen;
void init()
{
    dataLen[0] = 1;
    for (int i = 1; i < count; ++i)
    {
        int maxValue = 1;
        for (int t = 0; t<=i;++t)
        {
            if (data[i] > data[t])
            {
                if (maxValue < dataLen[t] + 1)
                {
                    maxValue = dataLen[t] + 1;
                }
            }
        }

        dataLen[i] = maxValue;
    }
}

int main()
{
    //while (scanf("%d", &count))
    scanf("%d", &count);


    data = new int[10001];
    dataLen = new int[10001];

    {
        for (int i = 0; i < count; ++i)
        {
            scanf("%d", data + i);
        }

        init();

        int value = 0;
        for (int i = 0; i < count; ++i)
        {
            if (value < dataLen[i])
            {
                value = dataLen[i];
            }
        }
        printf("%d\n", value);
    }

    delete[] data;
    delete[] dataLen;

	return 0;
}

#endif
#include <stdio.h>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;
const int maxSize = 101;
int Value[maxSize];
int Count[maxSize];
//char DataMap[100001];
int MAX(int x, int y)
{
    return x > y ? x : y;
}
int result = 0;

map<int, int> resultDict;
const int MaxM = 100000 + 1;
//int solve(vector<int>& vec, int n)
//{
//    if (n == 0)
//    {
//        return 0;
//    }
//    int t1 = solve(vec, n-1) + vec[n-1];
//    int t2 = solve(vec, n-1);
//
//    if(DataMap[t1] == 0)
//    {
//        DataMap[t1] = 1;
//        ++result;
//    }
//
//    if(DataMap[t2] == 0)
//    {
//        DataMap[t2] = 1;
//        ++result;
//    }
//
//    return MAX(t1, t2);
//}
//int solve(vector<int>& vec, int n, int space)
//{
//    vector<int> tmpSpace;
//    tmpSpace.resize(vec.size() + 1);
//    tmpSpace[0] = 0;
//    for (int k = 0; k<vec.size(); ++k)
//    {
//        //for(int i=1;i <= vec.size();++i)
//        for (int i = k + 1; i <= vec.size(); ++i)
//        {
//            int t1 = tmpSpace[i - 1];
//            int t2 = tmpSpace[i - 1] + vec[i - 1];
//            tmpSpace[i - 1] = MAX(t1, t2);
//            if (DataMap[tmpSpace[i]] == 0)
//            {
//                DataMap[tmpSpace[i]] = 1;
//                ++result;
//            }
//        }
//    }
//    return result;
//}

int solve(vector<int>& vec, int n, int space)
{
    if (space < 0)
    {
        return 0;
    }

    if (n == 0 || space == 0)
    {
        return 1;
    }

    int t = resultDict[n*MaxM + space];
    if (t)
    {
        return t;
    }

    int t1 = solve(vec, n - 1, space - vec[n-1]);   // take the n-th value
    int t2 = solve(vec, n - 1, space);              // not take the n-th value

    resultDict[n*MaxM + space] = t1 + t2;
    return t1+t2;
}

int dataSpace[MaxM + 1];
int minValue = -2147483640;
int solve2(vector<int>& vec, int n, int space)
{
    int maxResult = 0;
    memset(dataSpace, 0, sizeof(dataSpace));
    //for (int k = 0; k <= MaxM; ++k)
    //{
    //    dataSpace[k] = minValue;
    //}
    for (int i = 1; i <= n; ++i)
    {
        for (int k = 0; k <= space; ++k)
        {
            int r1 = 0;
            if (k >= vec[i - 1])
            {
                r1 = dataSpace[k - vec[i - 1]];
            }
            //else
            //{
            //    dataSpace[k] = 0;
            //    continue;
            //}
            if (i == 1 || k - vec[i - 1])
            {
                r1 = 1;
            }

            int r2 = dataSpace[k];
            if (i == 1)
            {
                r2 = 1;
            }
            dataSpace[k] = r1 + r2;

            if (maxResult < dataSpace[k])
            {
                maxResult = dataSpace[k];
            }
        }
    }

    return maxResult;
}

int main2()
{
    int n = 0;
    int m = 0;
    while (scanf("%d%d", &n, &m))
    {
        if (n == 0)
        {
            break;
        }
        for (int i = 0; i<n; ++i)
        {
            scanf("%d", Value + i);
        }
        for (int i = 0; i<n; ++i)
        {
            scanf("%d", Count + i);
        }
        vector<int> valueVec;
        vector<int> countVec;
        for (int i = 0; i < n; ++i)
        {
            int totalT = 0;
            int t = 1;
            while (t < Count[i] - totalT)
            {
                valueVec.push_back(Value[i] * t);
                totalT += t;
                t = t * 2;
            }
            t = t / 2;
            valueVec.push_back(Value[i] * (Count[i] - totalT));
        }

        result = 0;
        //memset(DataMap, 0, sizeof(DataMap));
        resultDict.clear();
        //int result = solve(valueVec, n, m);
        int result = solve(valueVec, valueVec.size(), m);
        printf("%d\n", result);
        printf("%d\n", result);
        printf("%d\n", result);
        for (map<int, int>::iterator iter = resultDict.begin(); iter != resultDict.end(); ++iter)
        {
            printf("%d %d, %d\n", iter->first / MaxM, iter->first % MaxM, iter->second);
        }
    }

    return 0;
}

#include <map>
#include <string>
const int MaxSize = 20;
int data[maxSize] = { 0 };

void init()
{
    for (int i = 0; i < MaxSize; ++i)
    {
        data[i] = i;
    }
}

int myMap[MaxSize] = { 0 };
void dfs(int total, int select, int start)
{
    if (select == 0)
    {
        for (int i = 0; i < total; ++i)
        {
            if (myMap[i])
            {
                printf("%d ", data[i]);
            }
        }

        puts("");        
        return;
    }

    for (int i = start; i < total - select; ++i)
    {
        myMap[i] = 1;
        dfs(total, select - 1, i + 1);
        myMap[i] = 0;
    }
}

int main()
{
    init();
    dfs(6,3,0);
}