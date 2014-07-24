#ifndef TESTSORTINGS_H
#define TESTSORTINGS_H

#include <vector>

#include "../common.h"
#include "countSort.h"

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
}

#endif // TESTSORTINGS_H
