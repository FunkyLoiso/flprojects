#ifndef TESTTREES_H
#define TESTTREES_H

#include <iostream>
#include "BinarySearchTree.h"
#include "AVLTree.h"

std::vector<int> tr;
bool traverseCollector(const int& key, const int&)
{
    tr.push_back(key);
    return false;
};

void testBST()
{
    std::vector<int> keys {12, 5, 18, 4, 45, 1, 0, 213, 555, 3, 78, 9, -5, 7};
    //BSTree
    BinarySearchTree<int> bst;
    bst.insert(keys);

    tr.clear();
    bst.traversePreorder(traverseCollector);
    assert( tr == (std::vector<int> {12, 5, 4, 1, 0, -5, 3, 9, 7, 18, 45, 213, 78, 555}) );
    tr.clear();
    bst.traverseInorder(traverseCollector);
    assert( tr == (std::vector<int> {-5, 0, 1, 3, 4, 5, 7, 9, 12, 18, 45, 78, 213, 555}) );
    tr.clear();
    bst.traversePostorder(traverseCollector);
    assert( tr == (std::vector<int> {-5, 0, 3, 1, 4, 7, 9, 5, 78, 555, 213, 45, 18, 12}) );

    bst.remove(4);
    tr.clear();
    bst.traversePreorder(traverseCollector);
    assert( tr == (std::vector<int> {12, 5, 1, 0, -5, 3, 9, 7, 18, 45, 213, 78, 555}) );
}

void testAVL()
{
    std::vector<int> keys {12, 5, 18, 4, 45, 1, 0, 213, 555, 3, 78, 9, -5, 7};

    //AVLTree
    AVLTree<int> avl;
    avl.insert(keys);

    tr.clear();
    avl.traversePreorder(traverseCollector);
    assert( tr == (std::vector<int> {12, 4, 1, 0, -5, 3, 7, 5, 9, 45, 18, 213, 78, 555}) );

    tr.clear();
    avl.traverseInorder(traverseCollector);
    assert( tr == (std::vector<int> {-5, 0, 1, 3, 4, 5, 7, 9, 12, 18, 45, 78, 213, 555}) );

    tr.clear();
    avl.traversePostorder(traverseCollector);
    assert( tr == (std::vector<int> {-5, 0, 3, 1, 5, 9, 7, 4, 18, 78, 555, 213, 45, 12}) );

    avl.remove(4);
    tr.clear();
    avl.traversePreorder(traverseCollector);
    assert( tr == (std::vector<int> {12, 5, 1, 0, -5, 3, 7, 9, 45, 18, 213, 78, 555}) );
}

void testTrees()
{
    testBST();
    testAVL();
//    std::vector<int> keys {12, 5, 3, 4, 17, 0, 9};
//    std::vector<int> keys {6, 3, 4, 5, 2, 17};
    std::vector<int> keys {12, 5, 18, 4, 45, 1, 0, 213, 555, 3, 78, 9, -5, 7};
    //BSTree
    BinarySearchTree<int> bst;
    bst.insert(keys);

    std::cout << bst.toString() << std::endl;
    bst.remove(4);
    std::cout << "removed 4:\n" << bst.toString() << std::endl;

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

    //AVLTree
    AVLTree<int> avl;
    std::cout << "AVLTree:" << std::endl;
    for(int key : keys)
    {
        avl.insert(key);
        std::cout << "inserted " << key << std::endl;
        std::cout << avl.toString() << std::endl;
    }
//    avl.insert(keys);
//    std::cout << avl.toString() << std::endl;

    avl.remove(4);
    std::cout << "removed 4:\n" << avl.toString() << std::endl;
}

#endif // TESTTREES_H
