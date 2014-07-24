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
    KosarajusSCCs.cpp

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
    sorting/radixSort.h

