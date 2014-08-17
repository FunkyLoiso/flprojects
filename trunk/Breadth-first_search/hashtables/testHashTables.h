#ifndef TESTHASHTABLES_H
#define TESTHASHTABLES_H

#include "HashTableLinkedList.h"
#include "HashTableOpen.h"

#include <string>
#include <iostream>
#include <unordered_map>
#include <chrono>

void testHashTables()
{
//    HashTableLinkedList<int, std::string> htll;
//    HashTableOpen<int, std::string> myHT;

//    for(int i = 1; i < 10; ++i)
//    {
//        std::cout << "adding " << i << std::endl;
//        myHT.insert(i, std::string("number ") + std::to_string(i));
//        myHT.dump();
//    }

//    for(int i = 0; i < 10; ++i)
//    {
//        std::string* p = myHT.find(i);
//        std::cout << i << ": " << (p != nullptr ? *p : "<nothing>") << std::endl;

//    }

//    std::cout << "erase 5 = " << myHT.erase(5) << std::endl;
//    myHT.dump();
//    std::cout << "erase 7 = " << myHT.erase(7) << std::endl;
//    myHT.dump();

//    for(int i = 0; i < 10; ++i)
//    {
//        std::string* p = myHT.find(i);
//        std::cout << i << ": " << (p != nullptr ? *p : "<nothing>") << std::endl;

//    }

    using namespace std::chrono;
    high_resolution_clock::time_point t1, t2;
    for(int i = 100000; i <= 10000000; i *= 10)
    {
        std::cout << std::endl << std::endl << i << " values." << std::endl;
        std::vector<int> values(i);
        for(auto& v : values) v = rand() * rand();

        //my
        {
            HashTableLinkedList<int, int> ht;
            std::cout << "my:" << std::endl;
            t1 = high_resolution_clock::now();
            for(int v : values) ht.insert(v, v);
            t2 = high_resolution_clock::now();
            std::cout << "addition :" << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;
            t1 = high_resolution_clock::now();
            for(int v : values) ht.find(v);
            t2 = high_resolution_clock::now();
            std::cout << "search  :" << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;
            t1 = high_resolution_clock::now();
            for(int v : values) ht.erase(v);
            t2 = high_resolution_clock::now();
            std::cout << "erasion :" << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;
        }

        //my open
        {
            HashTableOpen<int, int> ht;
            std::cout << "\nmy open:" << std::endl;
            t1 = high_resolution_clock::now();
            for(int v : values) ht.insert(v, v);
            t2 = high_resolution_clock::now();
            std::cout << "addition :" << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;
            t1 = high_resolution_clock::now();
            for(int v : values) ht.find(v);
            t2 = high_resolution_clock::now();
            std::cout << "search  :" << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;
            t1 = high_resolution_clock::now();
            for(int v : values) ht.erase(v);
            t2 = high_resolution_clock::now();
            std::cout << "erasion :" << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;
        }

        //std::unordered_map
        {
            std::unordered_map<int, int> map;
            std::cout << "\nstd::unordered_map" << std::endl;
            t1 = high_resolution_clock::now();
            for(int v : values) map.emplace(v, v);
            t2 = high_resolution_clock::now();
            std::cout << "addition :" << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;
            t1 = high_resolution_clock::now();
            for(int v : values) map.find(v);
            t2 = high_resolution_clock::now();
            std::cout << "search  :" << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;
            t1 = high_resolution_clock::now();
            for(int v : values) map.erase(v);
            t2 = high_resolution_clock::now();
            std::cout << "erasion :" << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;
        }

    }
}

#endif // TESTHASHTABLES_H
