#ifndef PARTIALSUM_H
#define PARTIALSUM_H

#include <future>
#include <vector>

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
}


template <class InIterator, class OutIterator, class BinaryOperation>
OutIterator partial_sum (InIterator first, InIterator last,
                         OutIterator result, BinaryOperation binary_op)
{
    const size_t numElements = last-first;
    if(numElements == 0) return result;
    //       const size_t jobsN = std::min(size_t(2), (numElements)/2);//temp
    //jobSize должен быть чётным
    //       const size_t jobSize = numElements/jobsN;
    //       std::vector<std::future<void>> futures;

    //       for(unsigned job = 0; job < jobsN; ++job)
    //       {
    //           InIterator begin = first + job*jobSize;
    //           OutIterator res = result + job*jobSize;
    //           InIterator end;
    //           if(job == jobsN-1) end = last;
    //           else end = begin + jobSize;

    ////           futures.push_back(std::async( __partial_sum_special_step1<InIterator, OutIterator, BinaryOperation>, begin, end, res, binary_op ));
    //           __partial_sum_special_step1(begin, end, res, binary_op);
    //       }
    //       for(auto& f : futures) f.get();
    //       futures.clear();
    //       __partial_sum_special_step1(first, last, result, binary_op);

    const size_t maxJobs = 5;// temp

    size_t step = 2;
    for(; step <= numElements; step <<= 1)
    {
        size_t jobSize = std::max(numElements / maxJobs, step);
        auto remainder = jobSize % step;
        if(remainder != 0)
        {

            jobSize -= remainder;//теперь jobSize больше step и кратен ему
        }
        //инварианты
        assert(jobSize >= step);
        assert(jobSize % step == 0);
        assert(jobSize <= numElements);
        auto jobsN = numElements / jobSize;

        for(unsigned job = 0; job < jobsN; ++job)
        {
            if(step == 2)
            {//на первом шаге не только запишем суммы с шагом 2, но так же скопируем все нечётные элементы
                InIterator begin = first + job*jobSize;
                InIterator end;
                if(job == jobsN - 1) end = last;
                else end = begin + jobSize;
                OutIterator res = result + job*jobSize;

                __partial_sum_special_step1(begin, end, res, binary_op);
            }
            else
            {
                OutIterator begin = first + job*jobSize;
                OutIterator end;
                if(job == jobsN -1) end = result + numElements;
                else end = begin + jobSize;
                __partial_sum_step(begin, end, binary_op, step);
            }
        }
    }

    //далее выполняем шаги в обратном направлении, но со сдвигом на пол шага вперёд. Если step + step/2 больше, чем размер коллекции, начинаем с меньшего шага.
    if(step + (step >> 1) > numElements) step >>= 1;
    for(; step >= 2; step >>= 1)
    {
        __partial_sum_step(result+(step>>1), result+numElements, binary_op, step);
    }
    return result;
}
}

#endif // PARTIALSUM_H
