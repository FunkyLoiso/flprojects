#ifndef TESTSTRINGMATCHING_H
#define TESTSTRINGMATCHING_H

#include "RabinKarp.h"
#include<iostream>

void testStringMatching()
{
//    std::string text = "no good dog in doggyland. Nothing";
//    std::string pattern = "o";
//    StringMatching::RabinKarp rk(text.begin(), text.end(), pattern.begin(), pattern.end());
//    std::string::iterator i;
//    while((i = rk.nextMatch()) != text.end())
//    {
//        std::cout << i - text.begin() << ", ";
//    }

    std::string text(1000000, 0);
    for(auto i = text.begin(); i != text.end(); ++i)
    {
        *i = rand()%256;
    }

    std::string pattern(2, 0);
    for(auto i = pattern.begin(); i != pattern.end(); ++i)
    {
        *i = rand()%256;
    }

    StringMatching::RabinKarp rk(text.begin(), text.end(), pattern.begin(), pattern.end());
    std::string::iterator i;
    while((i = rk.nextMatch()) != text.end())
    {
        std::cout << i - text.begin() << ", ";
    }
}

#endif // TESTSTRINGMATCHING_H
