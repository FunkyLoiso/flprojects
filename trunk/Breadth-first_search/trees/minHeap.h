#ifndef HEAP_H
#define HEAP_H

namespace HeapHelpers{}

namespace MinHeap
{

using namespace HeapHelpers;

//делает из коллекции min-heap
template<typename Iterator>
void make(Iterator begin, Iterator end)
{
    for(Iterator i = parent(begin, end-1);; --i)
    {
        heapify(begin, end, i);
        if(i == begin) break;
    }
}

//сортирует коллекцию
template<typename Iterator>
static void sort(Iterator begin, Iterator end)
{
    for(Iterator i = end-1;; --i)
    {
        std::iter_swap(begin, i);
        if(i == begin+1) break;
        heapify(begin, i, begin);
    }
}

//добавляет элемент из end-1 в кучу
template<typename Iterator>
static void push(Iterator begin, Iterator end)
{
    Iterator cur = end-1;
    Iterator p = parent(begin, cur);
    while(cur != begin && *p > *cur)
    {
        std::iter_swap(p, cur);
        cur = p;
        p = parent(begin, cur);
    }
}

//перемещает минимальный элемент в позицию end-1
template<typename Iterator>
static void pop(Iterator begin, Iterator end)
{
    std::iter_swap(begin, end-1);
    heapify(begin, end-1, begin);
}

}//namespace MinHeap

namespace HeapHelpers
{

template<typename Iterator>
Iterator parent(Iterator begin, Iterator i)
{
    size_t offset = i-begin;
    return begin + (offset - 1)/2;
}

template<typename Iterator>
Iterator left(Iterator begin, Iterator i)
{
    size_t offset = i-begin;
    return begin + (2 * offset) + 1;
}

template<typename Iterator>
Iterator right(Iterator begin, Iterator i)
{
    return left(begin, i) + 1;
}

template<typename Iterator>
void heapify(Iterator begin, Iterator end, Iterator i)
{
    Iterator lastParent = parent(begin, end-1);
    while(i <= lastParent)
    {
        Iterator l = left(begin, i);
        Iterator r = right(begin, i);
        Iterator smallest;

        if(l < end && *l < *i) smallest = l;
        else smallest = i;

        if(r < end && *r < *smallest) smallest = r;

        if(smallest != i)
        {
            std::iter_swap(i, smallest);
            i = smallest;
        }
        else break;
    }
}

}//namespace HeapHelpers

#endif // HEAP_H
