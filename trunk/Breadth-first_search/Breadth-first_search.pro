TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Graph.cpp \
    BFS.cpp \
    GraphVisitor.cpp \
    KruskalsMinimumSpanningTree.cpp \
    PrimsMinimumSpanningTree.cpp \
    ShortestPathDijkstra.cpp \
    KosarajusSCCs.cpp \
    Hoffman/HoffmanTree.cpp \
    Hoffman/CanonicalHoffmanCoDec.cpp

HEADERS += \
    Graph.h \
    BFS.h \
    GraphVisitor.h \
    KruskalsMinimumSpanningTree.h \
    PrimsMinimumSpanningTree.h \
    ShortestPathDijkstra.h \
    KosarajusSCCs.h \
    DisjointSet.h \
    trees/AVLTree.h \
    trees/BinarySearchTree.h \
    TestGraphs.h \
    trees/testTrees.h \
    sorting/countSort.h \
    sorting/testSortings.h \
    common.h \
    sorting/radixSort.h \
    sorting/bucketSort.h \
    trees/minHeap.h \
    dynamic/longestCommonSequence.h \
    dynamic/testDynamic.h \
    parallel/partialSum.h \
    parallel/testParallel.h \
    Hoffman/HoffmanTree.h \
    Hoffman/TestHoffman.h \
    Hoffman/CanonicalHoffmanCoDec.h \
    Hoffman/HoffmanArchiver.h \
    hashtables/HashTableLinkedList.h \
    hashtables/testHashTables.h

