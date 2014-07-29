#ifndef LONGESTCOMMONSEQUENCE_H
#define LONGESTCOMMONSEQUENCE_H

#include <vector>

#include "../common.h"

template<typename InIterator1, typename InIterator2, typename OutIterator>
size_t findLCS(InIterator1 b1, InIterator1 e1,
               InIterator2 b2, InIterator2 e2,
               OutIterator outBegin)
{
    size_t size1 = e1-b1;
    size_t size2 = e2-b2;

    //первый индекс - элемент первой коллекции. +1 для заполненных нулями крайних колонки и столбца
    //      + - - - - - > x
    //        c o l l 2
    //  +   0 0 0 0 0 0
    //  | c 0 1 1 1 1 1
    //  | o 0 1 2 2 2 2
    //  | l 0 1 2 3 3 3
    //  | l 0 1 2 3 4 4
    //y v 1 0 1 2 3 4 4
    std::vector<std::vector<size_t>> ls(size1+1, std::vector<size_t>(size2+1, 0));

    for(size_t y = 1; y <= size1; ++y)//индексы ls. y - элементы первой коллекции
    {
        for(size_t x = 1; x <= size2; ++x)//x - элементы второй коллекции
        {
            if(b1[y-1] == b2[x-1])
            {
                ls[y][x] = ls[y-1][x-1] + 1;
            }
            else if(ls[y][x-1] >= ls[y-1][x])
            {
                ls[y][x] = ls[y][x-1];
            }
            else
            {
                ls[y][x] = ls[y-1][x];
            }
        }
    }

    for(size_t y = size1, x = size2, outPos = ls[size1][size2]-1; x >= 1 && y >= 1;)
    {
        if(b1[y-1] == b2[x-1])
        {
            outBegin[outPos--] = b1[y-1];
            --x, --y;
        }
        else if(ls[y][x-1] >= ls[y-1][x])
        {
            --x;
        }
        else
        {
            --y;
        }
    }

    return ls[size1][size2];
}

#endif // LONGESTCOMMONSEQUENCE_H
