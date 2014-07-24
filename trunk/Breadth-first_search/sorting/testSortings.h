#ifndef TESTSORTINGS_H
#define TESTSORTINGS_H

#include <vector>
#include <random>

#include "../common.h"
#include "countSort.h"
#include "radixSort.h"
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
        int operator()(int val)
        {
            return (val >> m_byte*8) & 0xff;
        }

        int m_byte;
    };

    using namespace std::chrono;
    for(int count = 10000; count <= 100000000; count *= 10)
    {
        std::vector<unsigned int> intVec;
        for(int i = 0; i < count; ++i)
        {
            intVec.push_back(std::rand()*std::rand()*4);
        }
        decltype(intVec) tmpVec1(intVec), tmpVec2(intVec.size(), 0);

        std::cout << "\nsorting " << count << " integers" << std::endl;

        //radix
        std::vector<perByteHasher> hashers;
        for(int i = 0; i < 4; ++i) hashers.push_back(perByteHasher(i));//будем сортировать по каждому байту отдельно
        auto limits = std::make_pair(std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max());

        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        auto out = radixSort(&tmpVec1, &tmpVec2, limits, hashers);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();

        std::cout << "radix    : " << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;

        //std::sort
        t1 = high_resolution_clock::now();
        std::sort(intVec.begin(), intVec.end());
        t2 = high_resolution_clock::now();
        std::cout << "std::sort: " << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;

        assert(intVec == *out);
    }

}

#endif // TESTSORTINGS_H
