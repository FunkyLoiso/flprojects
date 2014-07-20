#ifndef TESTTREES_H
#define TESTTREES_H

#include <iostream>
#include "BinarySearchTree.h"
#include "AVLTree.h"

void testTrees()
{
//    std::vector<int> keys {12, 5, 3, 4, 17, 0, 9};
//    std::vector<int> keys {6, 3, 4, 5, 2, 17};
    std::vector<int> keys {12, 5, 18, 4, 45, 1, 0, 213, 555, 3, 78, 9, -5, 7};
    //BSTree
    BinarySearchTree<int> bst;
    bst.insert(keys);

    std::cout << bst.toString() << std::endl;

//    auto traversePrinter = [](const int& key, const int&)
//    {
//        std::cout << key << ", ";
//        return false;
//    };

//    std::cout << "\ntraversePreorder:  ";
//    bst.traversePreorder(traversePrinter);
//    std::cout << "\ntraverseInorder:   ";
//    bst.traverseInorder(traversePrinter);
//    std::cout << "\ntraversePostorder: ";
//    bst.traversePostorder(traversePrinter);
//    std::cout << std::endl;

//    BinarySearchTree<int> bst1 = bst;
//    bst1.remove(8);
//    std::cout << "removed 8:\n" << bst1.toString() << std::endl;
//    bst1 = bst;
//    bst1.remove(2);
//    std::cout << "removed 2:\n" << bst1.toString() << std::endl;
//    bst1 = bst;
//    bst1.remove(9);
//    std::cout << "removed 9:\n" << bst1.toString() << std::endl;

    //AVLTree
    AVLTree<int> avl;
    std::cout << "AVLTree:" << std::endl;
    avl.insert(keys);
    std::cout << avl.toString() << std::endl;

    std::cout << "remove 1:" << std::endl;
    avl.remove(1);
    std::cout << avl.toString() << std::endl;
//    avl.insert(3);
//    std::cout << avl.toString() << std::endl;
//    avl.insert(4);
//    std::cout << avl.toString() << std::endl;
}

#endif // TESTTREES_H
