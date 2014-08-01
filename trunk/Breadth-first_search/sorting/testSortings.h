#ifndef TESTSORTINGS_H
#define TESTSORTINGS_H

#include <vector>
#include <random>

#include "../common.h"
#include "countSort.h"
#include "radixSort.h"
#include "bucketSort.h"
#include "chrono"

void testSortings()
{
//    std::vector<double> v {1, 5.1, 5.2, 5.0, 9, 3, 1, 1, -17};
    std::vector<double> v {5.1, 5.2, 5.0};
    decltype(v) result(v.size());

    auto hasher = [](const decltype(v)::value_type& val) { return static_cast<int>(val*10); };
    countSort(v.begin(), v.end(), result.begin(), hasher);
    std::cout << "in:  ";
    printCollection(v);
    std::cout << "\nout: ";
    printCollection(result);

    //radixSort
    struct perByteHasher
    {
        perByteHasher(int byte) : m_byte(byte) {}
        int operator()(int val) const
        {
            return (val >> m_byte*8) & 0xff;
        }

        int m_byte;
    };

    std::vector<unsigned int> v333 {2, 17, 5};
    std::cout << "\nbefore bucketSort: ";
    printCollection(v333);
    bucketSort(v333.begin(), v333.end());
    std::cout << "\nafter bucketSort:  ";
    printCollection(v333);

    std::cout << "\nstd::thread::hardware_concurrency() is "  << std::thread::hardware_concurrency() << std::endl;
    using namespace std::chrono;
    for(int count = 1000000; count <= 100000000; count *= 10)
    {
        std::vector<unsigned int> intVec;
        for(int i = 0; i < count; ++i)
        {
            intVec.push_back(std::rand()*std::rand()*4);
        }

        std::cout << "\nsorting " << count << " integers" << std::endl;

        //radix
        decltype(intVec) tmpVec1(intVec), tmpVec2(intVec.size(), 0);
        std::vector<perByteHasher> hashers;
        for(int i = 0; i < 4; ++i) hashers.push_back(perByteHasher(i));//будем сортировать по каждому байту отдельно
        auto limits = std::make_pair(std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max());

        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        auto out = radixSort(&tmpVec1, &tmpVec2, limits, hashers);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();

        std::cout << "radix     : " << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;

        //parallel radix
        for(unsigned i = 1; i < std::thread::hardware_concurrency()*4; ++i)
        {
            unsigned jobsCount = i;
            decltype(intVec) tmpVec3(intVec), tmpVec4(intVec.size(), 0);
            t1 = high_resolution_clock::now();
         auto parallelOut = parallelRadixSort(&tmpVec3, &tmpVec4, limits, hashers, jobsCount);
            t2 = high_resolution_clock::now();
            std::cout << "p("<< jobsCount << ") radix: " << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;
        }

        //std::sort
        t1 = high_resolution_clock::now();
        std::sort(intVec.begin(), intVec.end());
        t2 = high_resolution_clock::now();
        std::cout << "std::sort : " << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;

        //bucketSort
//        decltype(intVec) bucketVec(intVec);
//        t1 = high_resolution_clock::now();
//        bucketSort(bucketVec.begin(), bucketVec.end());
//        t2 = high_resolution_clock::now();
//        std::cout << "bucketSort: " << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;

        assert(intVec == *out);
//        assert(intVec == *parallelOut);
//        assert(intVec == bucketVec);


    }

    //parallelCountSort
    std::vector<int> v19 {5, -7, 4, 9, 1, 0, 4, 5, 5};
    decltype(v19) result19(v19.size());

    auto intHasher = [](int val) { return val; };
    parallelCountSort(v19.begin(), v19.end(), result19.begin(), {-10, 10}, intHasher, 2);
    std::cout << "Parallel sort:" << std::endl;
    std::cout << "in : ";
    printCollection(v19);
    std::cout << "\nout: ";
    printCollection(result19);
}

#endif // TESTSORTINGS_H
