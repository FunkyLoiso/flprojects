#ifndef PARTIALSUM_H
#define PARTIALSUM_H

#include <future>
#include <vector>

#include <chrono>
#include <numeric>

using namespace std::chrono;

namespace parallel
{

//записывает в каждый step элемент его сумму с элементом, расположеным на step/2 позади
template <class Iterator, class BinaryOperation>
void __partial_sum_step (Iterator first, Iterator last,
                         BinaryOperation binary_op, size_t step)
{
    const size_t numElements = last-first;
    const size_t halfStep = step/2;

    for(size_t i = step-1; i < numElements; i += step)
    {
        first[i] = binary_op(first[i], first[i - halfStep]);
    }
//    std::cout << step << ": " << numElements << std::endl;
}

//копирует каждое нечётное значение без изменения, для каждого чётного записывает сумму с предыдущим
template <class InIterator, class OutIterator, class BinaryOperation>
void __partial_sum_special_step1 (InIterator first, InIterator last,
                                  OutIterator result, BinaryOperation binary_op)
{
    const size_t numElements = last-first;
    for(size_t i = 1; i < numElements; i+=2)
    {
        *result = first[i-1];
        *(result+1) = binary_op(first[i-1], first[i]);
        result += 2;
    }
//    std::cout << 2 << ": " << numElements << std::endl;
}


template <class InIterator, class OutIterator, class BinaryOperation>
OutIterator partial_sum (InIterator first, InIterator last,
                         OutIterator result, BinaryOperation binary_op, size_t maxJobs)
{
    const size_t numElements = last-first;
    if(numElements == 0) return result;

//    const size_t maxJobs = std::thread::hardware_concurrency();// temp
//    const size_t maxJobs = 1;
    std::vector<std::future<void>> futures;

    size_t step = 2;//текущий шаг суммирования (в каждый step элемент записывается его сумма с отстающим на step/2 назад). Минимальный размер задания равен step.
    for(; step <= numElements; step <<= 1)
    {
//        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        size_t jobSize = std::max(numElements / maxJobs, step);
        auto remainder = jobSize % step;
        if(remainder != 0)//размер задания должен быть кратен step
        {
            jobSize += (step - remainder); //увеличим размер задания до следующего числа, кратного step
            if(jobSize > numElements) jobSize -= step; // если размер привысил количество элементов, вернёмся к предыдущему кратному
//            jobSize -= remainder;//так тоже можно, но разделение получается неоптимальным, в последнем задании оказывается больше чем 2*jobSize элементов
        }
        auto jobsN = std::min(numElements / jobSize, maxJobs);
        //если число заданий не максимально, и остаток коллекции после jobsN заданий по jobSize больше step (то есть в нём есть что обрабатывать),
        //то выделим остаток в отдельное задание
        if(jobsN < maxJobs && numElements % jobSize >= step) ++jobsN;

        //инварианты
        assert(jobSize >= step);
        assert(jobSize % step == 0);
        assert(jobSize <= numElements);
        assert(jobsN <= maxJobs);

        for(unsigned job = 0; job < jobsN; ++job)
        {
            if(step == 2)
            {//на первом шаге не только запишем суммы с шагом 2, но так же скопируем все нечётные элементы
                InIterator begin = first + job*jobSize;
                InIterator end;
                if(job == jobsN - 1) end = last;
                else end = begin + jobSize;
                OutIterator res = result + job*jobSize;

//                __partial_sum_special_step1(begin, end, res, binary_op);
                futures.push_back( std::async(__partial_sum_special_step1<InIterator, OutIterator, BinaryOperation>, begin, end, res, binary_op) );

//                std::cout << std::this_thread::get_id() << ": " << end-begin << std::endl;
            }
            else
            {
                OutIterator begin = result + job*jobSize;
                OutIterator end;
                if(job == jobsN -1) end = result + numElements;
                else end = begin + jobSize;
//                __partial_sum_step(begin, end, binary_op, step);
                futures.push_back( std::async(__partial_sum_step<OutIterator, BinaryOperation>, begin, end, binary_op, step) );

//                std::cout << std::this_thread::get_id() << ": " << end-begin << std::endl;
            }
        }
       for(auto& f : futures) f.get();
       futures.clear();
//       high_resolution_clock::time_point t2 = high_resolution_clock::now();

//       std::cout << "step " << step << ": " << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;
    }

    //далее выполняем шаги в обратном направлении, но со сдвигом на пол шага вперёд. Если step + step/2 больше, чем размер коллекции, начинаем с меньшего шага.
    if(step + (step >> 1) > numElements) step >>= 1;
    for(; step >= 2; step >>= 1)
    {
        size_t jobSize = std::max(numElements / maxJobs, step);
        auto remainder = jobSize % step;
        if(remainder != 0)//размер задания должен быть кратен step
        {
            jobSize += (step - remainder); //увеличим размер задания до следующего числа, кратного step
            if(jobSize > numElements) jobSize -= step; // если размер привысил количество элементов, вернёмся к предыдущему кратному
//            jobSize -= remainder;//так тоже можно, но разделение получается неоптимальным, в последнем задании оказывается больше чем 2*jobSize элементов
        }
        auto jobsN = std::min(numElements / jobSize, maxJobs);
        //если число заданий не максимально, и остаток коллекции после jobsN заданий по jobSize больше step (то есть в нём есть что обрабатывать),
        //то выделим остаток в отдельное задание
        if(jobsN < maxJobs && numElements % jobSize >= step) ++jobsN;

        //инварианты
        assert(jobSize >= step);
        assert(jobSize % step == 0);
        assert(jobSize <= numElements);
        assert(jobsN <= maxJobs);

        for(unsigned job = 0; job < jobsN; ++job)
        {
            OutIterator begin = first + (step>>1) + job*jobSize;
            OutIterator end;
            if(job == jobsN -1) end = result + numElements;
            else end = begin + jobSize;

//            __partial_sum_step(begin, end, binary_op, step);
            futures.push_back( std::async(__partial_sum_step<OutIterator, BinaryOperation>, begin, end, binary_op, step) );
        }
        for(auto& f : futures) f.get();
        futures.clear();
    }
    return result;
}

template <class InIterator, class OutIterator, class BinaryOperation>
OutIterator partial_sum2 (InIterator first, InIterator last,
                         OutIterator result, BinaryOperation binary_op, size_t jobsCount)
{
    const size_t elCount = last - first;
    if(elCount < 1) return result;
//    const size_t jobsCount = std::thread::hardware_concurrency();
    const size_t jobSize = elCount/jobsCount;

    std::vector <std::future<OutIterator>> futures;

    for(unsigned job = 0; job < jobsCount; ++job)
    {
        InIterator begin = first + (job*jobSize);
        OutIterator res = result + (job*jobSize);
        InIterator end;
        if(job != jobsCount-1) end = begin + jobSize;
        else end = last;
        futures.push_back(std::async( std::partial_sum<InIterator, OutIterator, BinaryOperation>, begin, end, res, binary_op ));
//        std::partial_sum(begin, end, res, binary_op);
    }

    std::vector<OutIterator::value_type> sums;
    for(auto& f : futures)
    {
        sums.push_back(f.get()[-1]);
    }
    futures.clear();

    std::partial_sum(sums.begin(), sums.end(), sums.begin(), binary_op);

    for(unsigned job = 1; job < jobsCount; ++job)
    {
        OutIterator begin = result + (job*jobSize);
        OutIterator end;
        if(job != jobsCount-1) end = begin + jobSize;
        else end = result + elCount;

        const auto valToAdd = sums[job-1];
        auto adder = [valToAdd](const OutIterator::value_type& val) {return val + valToAdd;};
        futures.push_back(std::async( std::transform<OutIterator, OutIterator, decltype(adder)>, begin, end, begin, adder ));
//        std::transform(begin, end, begin, adder);
    }
    for(auto& f : futures)
    {
        f.get();
    }

    return result + elCount;
}

}// namespace parallel

#endif // PARTIALSUM_H
