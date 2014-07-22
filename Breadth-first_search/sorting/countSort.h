#ifndef COUNTSORT_H
#define COUNTSORT_H

#include <unordered_map>
#include <algorithm>
#include <vector>
#include <assert.h>

template<typename InIterator, typename OutIterator>
void countSort(InIterator inBegin, InIterator inEnd, OutIterator outBegin, std::pair<typename InIterator::value_type, typename InIterator::value_type> limits)
{
    using TVal = InIterator::value_type;
    static_assert(std::is_integral<TVal>::value && std::is_unsigned<TVal>::value, "Can only sort unsigned integral values");
    const auto min = limits.first;
    const auto max = limits.second;
    assert(min < max);

    std::vector<int> count(max-min+1, 0); // N
    for(InIterator i = inBegin; i != inEnd; ++i) // N
    {//посчитаем число появлений каждого значения
        ++count[(*i)-min];
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
        int& place = count[*i-min];//номер места для текущего значения
        *(outBegin + place-1) = *i;
        --place;//уменьшим номер места, чтобы значение, равные текущему, попало в предыдущее место
    }
}

//версия с автоматическим определением границ значений
template<typename InIterator, typename OutIterator>
void countSort(InIterator inBegin, InIterator inEnd, OutIterator outBegin)
{
    auto minmax = std::minmax_element(inBegin, inEnd);  // N
    countSort(inBegin, inEnd, outBegin, {*minmax.first, *minmax.second});
}


#endif // COUNTSORT_H
