#ifndef COUNTSORT_H
#define COUNTSORT_H

#include <unordered_map>
#include <algorithm>
#include <vector>
#include <functional>
#include <limits>
#include <assert.h>
#include <thread>
#include <future>

using namespace std::chrono;

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
        *(outBegin + place-1) = *i;//@todo это же должно сломаться, если place == 0?
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

/*
 * Параллельная версия
 */
template<typename InIterator, typename OutIterator, class Hasher>
void parallelCountSort(InIterator inBegin, InIterator inEnd, OutIterator outBegin,
               std::pair<long long int, long long int> limits,
               Hasher hasher, unsigned jobsCount = std::thread::hardware_concurrency())
{
    assert(jobsCount > 0);
    const auto min = limits.first;
    const auto max = limits.second;
    assert(min <= max);
    const size_t elCount = inEnd - inBegin;
    const size_t jobSize = elCount/jobsCount;

    std::vector<std::vector<int>*> counts(jobsCount);
    for(auto& p : counts) p = new std::vector<int>(max-min+1, 0); // N
    std::list<std::future<void>> futures;

    // 1
    //посчитаем число появлений каждого значения, разбив диапазон [inBegin; inEnd) на jobsCount частей
    auto counter = [=](InIterator begin, InIterator end, std::vector<int>* count)
    {
        for(InIterator i = begin; i != end; ++i) // N
        {
            auto hash = static_cast<long long int>(hasher(*i));
            ++(count->at(hash-min));
        }
    };

    for(unsigned job = 0; job < jobsCount; ++job)
    {
        InIterator begin = inBegin + (job*jobSize);
        InIterator end;
        if(job != jobsCount-1) end = begin + jobSize;
        else end = inEnd;
        futures.push_back(std::async(counter, begin,  end, counts.at(job)));
//        counter(begin, end, counts.at(job));
    }

    for(auto& f : futures)
    {
        f.get();
    }
    futures.clear();

    // 2
    // промежуточные суммы считаются очень быстро, в равнении с другими частями алгоритма, сделаем это в один поток.
    // Можно реализовать параллельный алгоритм, основанный на рекурсивном суммировании пар.
    int sum = 0;
    for(auto val = 0; val <= max-min; ++val)//для каждой колонки (возможного значения ключа)
    {
        for(auto arr : counts)
        {//последовательно в порядке возрастания номера job-а
            sum = (arr->at(val) += sum);//посчитаем и запишем кумулятивную сумму
        }
    }


    // 3
    //Обратный порядок для того, чтобы сортировка была стабильной
    //Пройдём по исходной коллекции и поместим каждое значение в соответствующую ячейку результирующей коллекции
    auto fill = [=] (InIterator end, InIterator begin, std::vector<int>* thisCount)
    {
        for(InIterator i = end; i != begin;)
        {
            --i;
            auto hash = static_cast<long long int>(hasher(*i));
            int& place = thisCount->at(hash-min);
            *(outBegin + place-1) = *i;
            --place;
        }
    };

    for(unsigned job = 0; job < jobsCount; ++job)
    {
        InIterator begin = inBegin + (job*jobSize);
        InIterator end;
        if(job != jobsCount-1) end = begin + jobSize;
        else end = inEnd;
        futures.push_back(std::async(fill, end,  begin, counts.at(job)));
//        fill(end, begin, job);
    }

    for(auto& f : futures)
    {
        f.get();
    }
    futures.clear();

    for(auto p : counts) delete p;
}


#endif // COUNTSORT_H
