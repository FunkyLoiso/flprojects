#ifndef TESTPARALLEL_H
#define TESTPARALLEL_H

#include "partialSum.h"

#include <vector>
#include <numeric>
#include <assert.h>
#include <chrono>

#include "../common.h"

void testParallel()
{
//    std::vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9};
//    std::vector<int> result {1, 3, 6, 10, 15, 21, 28, 36, 45};

//    for(int i = 0; i < 50; ++i)
//    {
//        std::vector<int> v(i);
//        std::iota(v.begin(), v.end(), 1);

//        std::cout << "\n\nin : ";
//        printCollection(v);
//        std::cout << std::endl;

//        std::vector<int> my(v);
//        parallel::partial_sum(my.begin(), my.end(), my.begin(), std::plus<int>());
//        std::cout << "\nout: ";
//        printCollection(my);

//        std::partial_sum(v.begin(), v.end(), v.begin());
//        assert(my == v);
//    }

    for(size_t count = 1000000000; count <= 1000000000; count *= 10)
    {
        std::vector<short> v(count);
//        std::for_each(v.begin(), v.end(), [](short& val) {val = std::rand();});
        std::iota(v.begin(), v.end(), 1);
        std::vector<short> stdCtrl(v);

        std::cout << "\npartial sums of " << count << " elements:" << std::endl;
        using namespace std::chrono;
        high_resolution_clock::time_point t1, t2;

        t1 = high_resolution_clock::now();
        std::partial_sum(stdCtrl.begin(), stdCtrl.end(), stdCtrl.begin(), std::plus<int>());
        t2 = high_resolution_clock::now();
        std::cout << "std : " << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;

        for(size_t jobsCount = 1; jobsCount <= std::thread::hardware_concurrency()*4; ++jobsCount)
        {
            std::vector<short> vCopy(v);
            t1 = high_resolution_clock::now();
            parallel::partial_sum2(vCopy.begin(), vCopy.end(), vCopy.begin(), std::plus<int>(), jobsCount);
            t2 = high_resolution_clock::now();
            std::cout << "my" << jobsCount << ": " << (duration_cast<milliseconds>(t2 - t1)).count() << " ms" << std::endl;

            if(vCopy != stdCtrl)
            {
                std::cout << "my != std!";
                exit(-1);
            }
        }
    }
    std::cout << "\npress return to exit";
    getchar();

}

#endif // TESTPARALLEL_H
