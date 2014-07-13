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
    ShortestPathDijkstra.cpp

HEADERS += \
    Graph.h \
    BFS.h \
    GraphVisitor.h \
    KruskalsMinimumSpanningTree.h \
    PrimsMinimumSpanningTree.h \
    ShortestPathDijkstra.h

