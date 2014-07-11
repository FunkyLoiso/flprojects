TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Graph.cpp \
    bfs.cpp \
    graphvisitor.cpp \
    shortestpathdijkstra.cpp

HEADERS += \
    Graph.h \
    bfs.h \
    graphvisitor.h \
    shortestpathdijkstra.h

