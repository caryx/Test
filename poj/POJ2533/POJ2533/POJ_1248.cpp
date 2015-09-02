// POJ2533.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <map>
using namespace std;
map<int, char> resultMap;
map<char, int> sortMap;

const int ToSelect = 5;
char tmpData[20] = { 0 };
char buf[20] = { 0 };
int target = 0;
int coff[27][ToSelect + 1] = { 0 };
int resultFound = 0;

void swap(int &x, int& y)
{
    int tmp = x;
    x = y;
    y = tmp;
}

void swap(char &x, char& y)
{
    char tmp = x;
    x = y;
    y = tmp;
}

void init()
{
    for (int i = 0; i <= 26; ++i)
    {
        for (int k = 0; k <= 5; ++k)
        {
            if (k <= 1)
            {
                coff[i][k] = i;
            }
            else
            {
                coff[i][k] = coff[i][k - 1] * i;
            }
        }
    }

    memset(tmpData, 0, sizeof(tmpData));
    resultFound = 0;
}

int checkResult(int x1, int x2, int x3, int x4, int x5)
{
    int result = (coff[x1][1] - coff[x2][2] + coff[x3][3] - coff[x4][4] + coff[x5][5]);
    return result == target;
}


void dfs(int len, int start, int toSelect)
{
    if (resultFound)
    {
        return;
    }

    if (!toSelect)
    {
        int resultData[ToSelect] = { 0 };
        int *p = resultData;
        for (int i = 0; i < len; ++i)
        {
            if (tmpData[i])
            {
                *p++ = i;
            }
        }

        char t = 'A' - 1;
        if (buf[0] == 'Y' - t  && buf[1] == 'O' - t  && buf[2] == 'X' - t  && buf[3] == 'U' - t && buf[4] == 'Z' - t)
        {
            buf[0] = 'Y' - t;
        }

        //if (checkResult(sortMap[buf[0]], sortMap[buf[1]], sortMap[buf[2]], sortMap[buf[3]], sortMap[buf[4]]))
        if (checkResult(buf[0], buf[1], buf[2], buf[3], buf[4]))
        {
            for (int i = 0; i < ToSelect; ++i)
            {
                //printf("%c", resultMap[resultData[i]]);
                //printf("%c", buf[resultData[i]]);
                printf("%c", buf[i] + 'A' - 1);
            }
            printf("\n");
            resultFound = 1;
            return;
        }


        ////if (checkResult(sortMap[buf[resultData[0]]], sortMap[buf[resultData[1]]], sortMap[buf[resultData[2]]], sortMap[buf[resultData[3]]], sortMap[buf[resultData[4]]]))
        //{
        //    for (int i = 0; i < ToSelect; ++i)
        //    {
        //        //printf("%c", resultMap[resultData[i]]);
        //        //printf("%c", buf[resultData[i]]);
        //        printf("%c", buf[i]);
        //    }
        //    printf("\n");
        //    //resultFound = 1;
        //    return;
        //}

        return;
    }

    //for (int i = start; i < len - toSelect; ++i)
    for (int i = start; i < len; ++i)
    {
        if (tmpData[i] == 0)
        {
            tmpData[start] = 1;
            swap(buf[start], buf[i]);
            dfs(len, start + 1, toSelect - 1);
            swap(buf[i], buf[start]);
            tmpData[start] = 0;
        }
    }
}

void solve()
{
    resultMap.clear();
    sortMap.clear();

    int strLen = strlen(buf);
    for (int i = 0; i < strLen; ++i)
    {
        resultMap[i] = buf[i];
        //sortMap[resultMap[i]] = i+1;
        sortMap[buf[i]] = i + 1;
    }

    int index = 0;
    for (map<char, int>::reverse_iterator iter = sortMap.rbegin(); iter != sortMap.rend(); ++iter)
    {
        buf[index++] = iter->first - 'A' + 1;
    }

    dfs(strLen, 0, ToSelect);
}

//11700519 ZAYEXIWOVU
int main()
{
    init();
    while (scanf("%d%s", &target, buf))
    {
        if (target == 0 && buf[0] == 'E' && buf[1] == 'N' && buf[2] == 'D')
        {
            break;
        }

        memset(tmpData, 0, sizeof(tmpData));
        solve();
        if (resultFound == 0)
        {
            printf("no solution\n");
        }
        resultFound = 0;
    }
    return 0;
}


