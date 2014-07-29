#ifndef TESTDYNAMIC_H
#define TESTDYNAMIC_H

#include <vector>
#include <algorithm>
#include "longestCommonSequence.h"
#include "../common.h"
#include <cassert>

void testLCS()
{
    std::vector<std::string> s1 {"#include \"TestGraphs.h\"\n",
                                 "#include \"trees/testTrees.h\"\n",
                                 "#include \"sorting/testSortings.h\"\n",
                                 "\n",
                                 "int main()\n",
                                 "{\n",
                                 "//    testGraphs();\n",
                                 "    testTrees();\n",
                                 "//    testSortings();\n",
                                 "\n",
                                 "    return 0;\n",
                                 "}\n",
                                 ""};
    std::vector<std::string> s2 {"//#include \"TestGraphs.h\"\n",
                                 "//#include \"trees/testTrees.h\"\n",
                                 "//#include \"sorting/testSortings.h\"\n",
                                 "#include \"dynamic/testDynamic.h\"\n",
                                 "\n",
                                 "int main()\n",
                                 "{\n",
                                 "//    testGraphs();\n",
                                 "//    testTrees();\n",
                                 "//    testSortings();\n",
                                 "    testDynamic();\n",
                                 "\n",
                                 "    return 0;\n",
                                 "}\n",
                                 ""};

    std::vector<std::string> same {"\n",
                                   "int main()\n",
                                   "{\n",
                                   "//    testGraphs();\n",
                                   "//    testSortings();\n",
                                   "\n",
                                   "    return 0;\n",
                                   "}\n",
                                   ""};

    std::vector<std::string> result(std::max(s1.size(), s2.size()));

    size_t length = findLCS(s1.begin(), s1.end(), s2.begin(), s2.end(), result.begin());
    result.resize(length);
    assert(length == same.size());
    assert(result == same);
}

void testDynamic()
{
    testLCS();

    std::vector<int> s1 {1, 5, 8, 16};
    std::vector<int> s2 {5, 1, 8, 7, 16};
    std::vector<int> result(std::max(s1.size(), s2.size()));

    size_t length = findLCS(s1.begin(), s1.end(), s2.begin(), s2.end(), result.begin());

    std::cout << "LCS of\n1: ";
    printCollection(s1);
    std::cout << "\n2: ";
    printCollection(s2);
    std::cout << "\nis ";
    result.resize(length);
    printCollection(result);
    std::cout << "\nIt has " << length << " element" << (length != 1 ? "s" : "") << std::endl;
}

#endif // TESTDYNAMIC_H
