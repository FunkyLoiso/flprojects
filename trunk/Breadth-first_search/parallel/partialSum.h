#ifndef PARTIALSUM_H
#define PARTIALSUM_H

#include <future>
#include <vector>

namespace parallel
{

//записывает в каждый step элемент его сумму с элементом, расположеным на step/2 позади
template <class Iterator, class BinaryOperation>
   void __partial_sum_step (Iterator first, Iterator last,
                            BinaryOperation binary_op, unsigned step)
   {
       const auto numElements = last-first;
       unsigned halfStep = step/2;

       for(auto i = step-1; i < numElements; i += step)
       {
           first[i] = binary_op(first[i], first[i - halfStep]);
       }
   }

   //копирует каждое нечётное значение без изменения, для каждого чётного записывает сумму с предыдущим
template <class InIterator, class OutIterator, class BinaryOperation>
  void __partial_sum_special_step1 (InIterator first, InIterator last,
                                    OutIterator result, BinaryOperation binary_op)
  {
      const auto numElements = last-first;
      for(auto i = 1; i < numElements; i+=2)
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

       unsigned step = 2;
       for(; step <= numElements; step <<= 1)
       {
           if(step == 2)
           {//на первом шаге не только запишем суммы с шагом 2, но так же скопируем все нечётные элементы
               __partial_sum_special_step1(first, last, result, binary_op);
           }
           else
           {
               __partial_sum_step(result, result+numElements, binary_op, step);
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
