#ifndef RADIXSORT_H
#define RADIXSORT_H

#include "countSort.h"

template<typename Collection, class Hasher>
Collection* radixSort(Collection* c1, Collection* c2,
                      std::pair<long long int, long long int> limits,
                      std::vector<Hasher> hashers)
{

    const auto min = limits.first;
    const auto max = limits.second;
    assert(min <= max);

    auto* curSrc = c1;
    auto* curDst = c2;
    for(const Hasher& hasher : hashers)
    {
        countSort(curSrc->begin(), curSrc->end(), curDst->begin(), {min, max}, hasher);
        std::swap(curSrc, curDst);
    }
    return curSrc;//потому что после последней сортировки тоже произошёл вызов swap
}

template<typename Collection, class Hasher>
Collection* parallelRadixSort(Collection* c1, Collection* c2,
                              std::pair<long long int, long long int> limits,
                              std::vector<Hasher> hashers, unsigned jobsCount = std::thread::hardware_concurrency())
{

    const auto min = limits.first;
    const auto max = limits.second;
    assert(min <= max);

    auto* curSrc = c1;
    auto* curDst = c2;
    for(const Hasher& hasher : hashers)
    {
        parallelCountSort(curSrc->begin(), curSrc->end(), curDst->begin(), {min, max}, hasher, jobsCount);
        std::swap(curSrc, curDst);
    }
    return curSrc;//потому что после последней сортировки тоже произошёл вызов swap
}

#endif // RADIXSORT_H
