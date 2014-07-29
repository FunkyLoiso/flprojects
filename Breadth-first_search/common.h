#ifndef COMMON_H
#define COMMON_H

#include <iostream>

template<typename C>
void printCollection2(const C& coll)
{
    if(coll.size() == 0) std::cout << "<empty>";
    else
    {
        for(auto val : coll)
        {
            printCollection(val);
            std::cout << std::endl;
        }
    }
}

template<typename C>
void printCollection(const C& coll)
{
    if(coll.size() == 0) std::cout << "<empty>";
    else
    {
        for(auto val : coll)
        {
            std::cout << val << ", ";
        }
    }
}

#endif // COMMON_H
