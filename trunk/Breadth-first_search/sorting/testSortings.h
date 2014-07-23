#ifndef TESTSORTINGS_H
#define TESTSORTINGS_H

#include <vector>

#include "../common.h"
#include "countSort.h"

void testSortings()
{
    std::vector<int> v {1, 5, 8, 11, 9, 3, 1, 1, -17};
    std::vector<int> result(v.size());
    countSort(v.begin(), v.end(), result.begin());
    std::cout << "in:  ";
    printCollection(v);
    std::cout << "\nout: ";
    printCollection(result);
}

#endif // TESTSORTINGS_H
