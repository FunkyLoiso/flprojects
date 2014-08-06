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

    for(int i = 0; i < 50; ++i)
    {
        std::vector<int> v(i);
        std::iota(v.begin(), v.end(), 1);

        std::cout << "\n\nin : ";
        printCollection(v);
        std::cout << std::endl;

        std::vector<int> my(v);
        parallel::partial_sum(my.begin(), my.end(), my.begin(), std::plus<int>());
        std::cout << "\nout: ";
        printCollection(my);

        std::partial_sum(v.begin(), v.end(), v.begin());
        assert(my == v);
    }


}

#endif // TESTPARALLEL_H
