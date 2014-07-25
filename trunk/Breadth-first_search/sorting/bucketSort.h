#ifndef BUCKETSORT_H
#define BUCKETSORT_H

#include <vector>
#include <list>

template<typename InIterator>
void bucketSort(InIterator begin, InIterator end,
               std::pair<typename InIterator::value_type, typename InIterator::value_type> limits,
               size_t numBuckets = 0)
{
    const auto min = limits.first;
    const auto max = limits.second;
    const size_t range = max-min;
    const uint64_t numElements = end - begin;
    if(numBuckets == 0) numBuckets = numElements;

    //распределим значения между бакетами
    std::vector<std::list<InIterator::value_type>> buckets(numBuckets); // N
    for(InIterator i = begin; i != end; ++i) // N
    {
        auto& value = *i;
        auto& bucket = buckets.at((numBuckets-1) * (value-min) / range);
        if(bucket.empty()) bucket.push_back(value); // C
        else
        {
            bool found = false;
            for(auto bi = bucket.begin(); bi != bucket.end(); ++bi)// N, N -> 1
            {
                if(*bi > value)
                {
                    bucket.insert(bi, value);
                    found = true;
                    break;
                }
            }
            if(!found) bucket.push_back(value);
        }
    }

    //поочерёдно извлечём все значения из бакетов
    InIterator i = begin;
    for(auto& bucket : buckets)// N
    {
        for(auto& value : bucket)// N, N -> 1
        {
            *i = value;
            ++i;
        }
    }
}

template<typename InIterator>
void bucketSort(InIterator begin, InIterator end, size_t numBuckets = 0)
{
    auto min = std::numeric_limits<InIterator::value_type>::max();
    auto max = std::numeric_limits<InIterator::value_type>::min();

    for(auto i = begin; i != end; ++i) // N
    {
        auto val = *i;
        if(min > val) min = val;
        if(max < val) max = val;
    }

    bucketSort(begin, end, {min, max}, numBuckets);
}

#endif // BUCKETSORT_H
