#ifndef BFS_H
#define BFS_H

#include <set>
#include <list>

#include "Graph.h"
#include "graphvisitor.h"

class BFS
{
public:
    BFS(const UndirectedGraph& graph);

    std::set<int> connectedComponent(int start) const;
    std::list<int> shortestPath(int from, int to) const;

private:
    void doSearch(int start, GraphVisitor& visitor) const;
    const UndirectedGraph& m_graph;
};

#endif // BFS_H
