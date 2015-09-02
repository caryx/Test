// POJ2533.cpp : Defines the entry point for the console application.


//

#include "stdafx.h"

#include <stdio.h>
#include <map>
#include <string>
using namespace std;
const int MaxCharCount = 5;
char baseBuf[MaxCharCount + 1] = { 0 };
char buf[MaxCharCount+1] = { 0 };
char tmpBuf[MaxCharCount+1] = { 0 };
char charMap[26] = { 0 };

void init()
{
    memset(charMap, 0, sizeof(charMap));
    for (int i = 0; i < MaxCharCount; ++i)
    {
        ++charMap[baseBuf[i] - 'A'];
    }
}

int isUpperCase(char a)
{
    if (a >= 'A' && a <= 'Z')
    {
        return 1;
    }

    return 0;
}

int hasChar(char a)
{
    if (isUpperCase(a))
    {
        return charMap[a - 'A'];
    }

    return 0;
}

int isValid(string& str)
{
    if (str.size() == MaxCharCount)
    {
        for (int i = 0; i < MaxCharCount; ++i)
        {
            if (!isUpperCase(str[i]))
            {
                return 0;
            }
        }

        return 1;
    }

    return 0;
}

int main()
{
    int keepLooping = 1;
    while (keepLooping)
    {
        char* p = gets(baseBuf);
        string baseStr(baseBuf);
        if (baseStr == "LINGO")
        {
            break;
        }

        if (baseStr == "")
        {
            continue;
        }

        printf("%c....\n", baseBuf[0]);
        for (int i = 0; i < MaxCharCount; ++i)
        {
            tmpBuf[i] = '.';
        }
        tmpBuf[0] = baseBuf[0];
        init();

        int count = 0;
        int gameOver = 0;
        while (1)
        {
            init();
            ++count;
            gets(buf);
            string guessStr(buf);

            if (guessStr == "")
            {
                break;
            }

            if (gameOver)
            {
                continue;
            }

            //if (guessStr.size() != 5)
            if (!isValid(guessStr))
            {
                printf("%s\n", tmpBuf);
                continue;
            }

            if (guessStr == baseStr)
            {
                printf("%s\n", buf);
                gameOver = 1;
                continue;
            }

            if (count < 6)
            {

                memset(tmpBuf, 0, sizeof(tmpBuf));
                for (int i = 0; i < 5; ++i)
                {
                    if (hasChar(buf[i]) && (buf[i] == baseStr[i]))
                    {
                        tmpBuf[i] = buf[i];
                        --charMap[buf[i] - 'A'];
                    }
                }

                for (int i = 0; i < 5; ++i)
                {
                    if (tmpBuf[i])
                    {
                        continue;
                    }
                    if (hasChar(buf[i]))
                    {
                        if (buf[i] != baseStr[i])
                        {
                            tmpBuf[i] = buf[i] + ('a' - 'A');
                            --charMap[buf[i] - 'A'];
                        }
                    }
                    else
                    {
                        tmpBuf[i] = '.';
                    }
                }

                printf("%s\n", tmpBuf);
            }
            else
            {
                for (int i = 0; i < MaxCharCount; ++i)
                {
                    printf("%c", baseStr[i] - 'A' + 'a');
                }

                printf("\n");
                gameOver = 1;
            }

        }
    }

    return 0;
}

