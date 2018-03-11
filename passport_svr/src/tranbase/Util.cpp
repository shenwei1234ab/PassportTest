/*
 * Util.cpp
 *
 *  Created on: 2015年11月2日
 *      Author: leave
 */

#include "tranbase/Util.h"


int StrListToIntArray(const char* pSrcStr, int* pDesArray, int Max, const char* cSplitChar)
{
    static const int MaxBufLen = 256;
    int iSrcStrLen = strlen(pSrcStr);

    if (iSrcStrLen + 1 > MaxBufLen)
    {
        return -1;
    }

    char tempString[MaxBufLen];
    memset(tempString, 0, MaxBufLen);
    strncpy(tempString, pSrcStr, iSrcStrLen);
    int num = 0;

    // strsep may be better
    char* tempStrOut = strtok(tempString, cSplitChar);

    while (tempStrOut != NULL)
    {
        if (num >= Max)
        {
            return -2;
        }
        pDesArray[num] = atoi(tempStrOut);
        num++;
        tempStrOut = strtok(NULL, cSplitChar);
    }

    return num;
}
