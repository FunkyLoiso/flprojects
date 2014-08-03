#ifndef PARTIALSUM_H
#define PARTIALSUM_H

namespace parallel
{

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
       const auto numElements = last-first;
       __partial_sum_special_step1(first, last, result, binary_op);

       unsigned step = 4;
       for(; step <= numElements; step <<= 1)
       {
           __partial_sum_step(result, result+numElements, binary_op, step);
       }

       for(; step >= 2; step >>= 1)
       {
           if(step + (step>>1) > numElements) continue;
           __partial_sum_step(result+(step>>1), result+numElements, binary_op, step);
       }
        return result;
   }
}

#endif // PARTIALSUM_H
