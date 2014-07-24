#ifndef COUNTSORT_H
#define COUNTSORT_H

#include <unordered_map>
#include <algorithm>
#include <vector>
#include <functional>
#include <limits>
#include <assert.h>

template<typename InIterator, typename OutIterator, class Hasher>
void countSort(InIterator inBegin, InIterator inEnd, OutIterator outBegin,
               std::pair<long long int, long long int> limits,
               Hasher hasher)
{
    const auto min = limits.first;
    const auto max = limits.second;
    assert(min <= max);

    std::vector<int> count(max-min+1, 0); // N
    for(InIterator i = inBegin; i != inEnd; ++i) // N
    {//посчитаем число появлений каждого значения
        auto hash = static_cast<long long int>(hasher(*i));
        ++count[hash-min];
    }

    for(auto i = count.begin(); i != count.end()-1; ++i) // N
    {//поместим в каждую ячейку count кумулятивную сумму предыдущих ячеек.
        *(i+1) += *i;
    }

    //Обратный порядок для того, чтобы сортировка была стабильной
    //Пройдём по исходной коллекции и поместим каждое значение в соответствующую ячейку результирующей коллекции
    for(InIterator i = inEnd; i != inBegin;) // N
    {
        --i;
        auto hash = static_cast<long long int>(hasher(*i));
        int& place = count[hash-min];//номер места для текущего значения
        *(outBegin + place-1) = *i;
        --place;//уменьшим номер места, чтобы значение, равные текущему, попало в предыдущее место
    }
}

//Версия с автоматическим определением границ значений
//hasher должен возвращать что-то, приводимое к long long int
template<typename InIterator, typename OutIterator, class Hasher>
void countSort(InIterator inBegin, InIterator inEnd, OutIterator outBegin, Hasher hasher)
{
    auto min = std::numeric_limits<long long int>::max();
    auto max = std::numeric_limits<long long int>::min();

    for(auto i = inBegin; i != inEnd; ++i) // N
    {
        auto val = static_cast<long long int>(hasher(*i));
        if(min > val) min = val;
        if(max < val) max = val;
    }

    countSort(inBegin, inEnd, outBegin, {min, max}, hasher);
}


#endif // COUNTSORT_H
