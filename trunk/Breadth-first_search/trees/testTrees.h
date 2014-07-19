#ifndef TESTTREES_H
#define TESTTREES_H

#include <iostream>
#include "BinarySearchTree.h"
#include "AVLTree.h"

void testTrees()
{
    BinarySearchTree<int> bst;
//    bst.insert(12, 12);
//    bst.insert(5, 12);
//    bst.insert(18, 12);
//    bst.insert(4, 7);
//    bst.insert(45, 7);
//    bst.insert(5, 7);
//    bst.insert(1, 7);
//    bst.insert(0, 7);
//    bst.insert(213, 7);
//    bst.insert(4, 7);
//    bst.insert(555, 7);
//    bst.insert(3, 7);
//    bst.insert(78, 7);
//    bst.insert(9, 7);
//    bst.insert(-5, 7);
//    bst.insert(7);
    bst.insert({8, 5, 10, 2, 9, 7, 15, 1});

    std::cout << bst.toString();

    auto traversePrinter = [](const int& key, const int&)
    {
        std::cout << key << ", ";
        return false;
    };

    std::cout << "\ntraversePreorder:  ";
    bst.traversePreorder(traversePrinter);
    std::cout << "\ntraverseInorder:   ";
    bst.traverseInorder(traversePrinter);
    std::cout << "\ntraversePostorder: ";
    bst.traversePostorder(traversePrinter);
    std::cout << std::endl;

    BinarySearchTree<int> bst1 = bst;
    bst1.remove(8);
    std::cout << "removed 8:\n" << bst1.toString() << std::endl;
    bst1 = bst;
    bst1.remove(2);
    std::cout << "removed 2:\n" << bst1.toString() << std::endl;
    bst1 = bst;
    bst1.remove(9);
    std::cout << "removed 9:\n" << bst1.toString() << std::endl;
}

#endif // TESTTREES_H
