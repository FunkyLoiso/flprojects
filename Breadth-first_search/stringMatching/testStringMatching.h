#ifndef TESTSTRINGMATCHING_H
#define TESTSTRINGMATCHING_H

#include "RabinKarp.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std::chrono;

void testStringMatching()
{
//    std::string text = "Oh good dog in doggyland. Nothing";
//    std::string pattern = "good dog";
//    StringMatching::RabinKarp rk(text.begin(), text.end(), pattern.begin(), pattern.end());
//    std::string::iterator i;
//    while((i = rk.nextMatch()) != text.end())
//    {
//        std::cout << i - text.begin() << ", ";
//    }
    {
        std::string text(10000000, 0);
        for(auto i = text.begin(); i != text.end(); ++i)
        {
            *i = rand()%10;
        }

        std::string pattern(5, 0);
        for(auto i = pattern.begin(); i != pattern.end(); ++i)
        {
            *i = rand()%10;
        }

        // Рабин-Карп
        StringMatching::RabinKarp rk(text.begin(), text.end(), pattern.begin(), pattern.end());
        std::vector<int> shiftsRK;
        std::string::iterator i;
        while((i = rk.nextMatch()) != text.end())
        {
                    std::cout << i - text.begin() << ", ";
            shiftsRK.push_back(i - text.begin());
        }

        // std
        std::vector<int> shiftsSTD;
        i = text.begin();
        while( (i = std::search(i, text.end(), pattern.begin(), pattern.end())) != text.end() )
        {
            shiftsSTD.push_back(i - text.begin());
            ++i;
        }

        std::cout << "Rabin-Karp test: " << (shiftsSTD == shiftsRK ? "passed" : "failed") << std::endl;
    }

    // тест производительности
    for(int patternLength = 1; patternLength <= 4096; patternLength *= 2)
    {
        for(int count = 1000000; count <= 100000000; count *= 10)
        {
            std::cout << std::endl << "searching " << patternLength << "-char pattern in " << count << "-char string" << std::endl;
            std::string text(count, 0);
            for(auto i = text.begin(); i != text.end(); ++i)
            {
                *i = rand()%4;
            }

            std::string pattern(patternLength, 0);
            for(auto i = pattern.begin(); i != pattern.end(); ++i)
            {
                *i = rand()%4;
            }

            // std
            std::string::iterator i = text.begin();
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            while( (i = std::search(i, text.end(), pattern.begin(), pattern.end())) != text.end() )
            {
                ++i;
            }
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            std::cout << "std       : " << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;

            // Рабин-Карп
            StringMatching::RabinKarp rk(text.begin(), text.end(), pattern.begin(), pattern.end());
            t1 = high_resolution_clock::now();
            while((i = rk.nextMatch()) != text.end())
            {
            }
            t2 = high_resolution_clock::now();
            std::cout << "Rabin-Karp: " << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;
        }
    }
}

#endif // TESTSTRINGMATCHING_H
