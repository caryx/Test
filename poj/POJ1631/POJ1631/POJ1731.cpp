// POJ1631.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
//#define POJ1730
#ifdef POJ1730
#include<iostream>
#include<algorithm>
#include <vector>
#include <string>

using namespace std;

using namespace std;
char in[300], l[300], o[300];
vector<string> result1;
void f(int d)
{
    char c = '\0';
    if (d == strlen(in))
    {
        o[d] = '\0';
        //cout << o << endl;
        result1.push_back(string(o));
    }
    for (int i = 0; i<strlen(in); i++)
        if (!l[i] && in[i] != c)
        {
        o[d] = in[i];
        l[i] = 1;
        c = in[i];
        f(d + 1);
        l[i] = 0;
        }
}

void test()
{
    result1.clear();
    {
        sort(in, in + strlen(in));
        memset(l, 0, sizeof(l));
        f(0);
    }
}


vector<string> result2;

bool isSame()
{
    if (result1.size() == result2.size())
    {
        for (int i = 0; i < result1.size(); ++i)
        {
            if (result1[i] != result2[i])
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

void dumpResult()
{
    for (int i = 0; i < result1.size(); ++i)
    {
        std::cout << result1[i] << endl;
    }

    cout << endl;

    for (int i = 0; i < result2.size(); ++i)
    {
        std::cout << result2[i] << endl;
    }
}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int maxSize = 301;
char data[maxSize] = { 0 };
char next[maxSize] = { 0 };

int compare(const void* p1, const void* p2)
{
    return *(char*)p1 > *(char*)p2;
}

void myorder(int start, int end)
{
    if (start >= end)
    {
        //puts(data);
        result2.push_back(string(data));
        return;
    }

    myorder(start + 1, end);
    qsort(data, end, 1, compare);

    for (int i = start+1; i < end; ++i)
    {
        if (data[i] != data[start])
        {
            char t = data[start];
            data[start] = data[i];
            data[i] = t;
            //puts(data);
            myorder(start + 1, end);
        }
    }    
}

//void normalOrder(int start, int end)
//{
//    if (start >= end)
//    {
//        return;
//    }
//
//    normalOrder(start + 1, end);
//    for (int i = start; i < end; ++i)
//    {
//        int t = data[i];
//        if (i != start)
//        {
//            data[i] = data[start];
//            data[start] = t;
//        //}
//
//        puts(data);
//        normalOrder(start + 1, end);
//
//        //if (i != start)
//        //{
//            data[start] = data[i];
//            data[i] = t;
//        }
//        else
//        {
//            puts(data);
//        }
//    }
//}

void normalOrder2(int start, int end)
{
    if (start >= end)
    {
        puts(data);
        return;
    }

    for (int i = start; i < end; ++i)
    {
        int t = data[i];

        if (i != start && data[i] == data[start])
        {
            continue;
        }

        if (i - 1 >= 0 && data[i] == data[i - 1])
        {
            continue;
        }



        data[i] = data[start];
        data[start] = t;

        char tmpData[maxSize] = { 0 };
        memcpy(tmpData, data, sizeof(data));
        qsort(data + i, end-i, 1, compare);
        //puts(data);
        normalOrder2(start + 1, end);
        memcpy(data, tmpData, sizeof(data));

        data[start] = data[i];
        data[i] = t;
    }
}


void normalOrder(int start, int end)
{
    if (start >= end)
    {
        puts(data);
        //result2.push_back(string(data));
        return;
    }

    for (int i = start; i < end; ++i)
    {
        char t = data[i];

        //if (i != start && data[i] == data[start])
        //{
        //    continue;
        //}

        if (i != start && data[i] == data[start])
        {
            continue;
        }

        if (i != start && data[i] == data[i-1])
        {
            continue;
        }



        data[i] = data[start];
        data[start] = t;

        char tmpData[maxSize] = { 0 };
        memcpy(tmpData, data, sizeof(data));
        qsort(data + start + 1, end - start - 1, 1, compare);
        normalOrder(start + 1, end);
        memcpy(data, tmpData, sizeof(data));

        data[start] = data[i];
        data[i] = t;
    }
}

#include<algorithm>
int main()
{
    int count;
    while (scanf("%s", data))
    {
        int len = strlen(data);
        qsort(data, len, 1, compare);
        sort(data, data + len);
        result2.clear();
        normalOrder(0, len);
        //memcpy(in, data, sizeof(data));
        //test();
        //if (!isSame())
        //{
        //    dumpResult();
        //}

        //cout << "done" << endl;
    }
    return 0;
}
int len;
char target[maxSize] = { 0 };
char flag[maxSize] = { 0 };
void caryOrder(int value)
{
    if (value == len)
    {
        target[value] = 0;
        puts(target);
        //return;
    }

    char lastChar = 0;
    for (int i = 0; i < len; ++i)
    {
        if (!flag[i] && data[i] != lastChar)
        {
            flag[i] = 1;
            lastChar = data[i];
            target[value] = data[i];
            caryOrder(value + 1);
            flag[i] = 0;
        }
    }
}

int main3()
{
    int count;
    //while (scanf("%s", data))
    while (cin >> data)
    {
        memset(flag, 0, sizeof(flag));
        len = strlen(data);
        //qsort(data, len, 1, compare);
        sort(data, data + len);

        caryOrder(0);

        //printf("%s\n", data);
    }
    return 0;
}


#endif
