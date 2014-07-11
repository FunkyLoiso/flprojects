#ifndef BFS_H
#define BFS_H

#include <set>
#include <list>

#include "Graph.h"
#include "graphvisitor.h"

class BFS
{
public:
    BFS(const DirectedWeightedGraph& graph);

    std::set<int> reachableComponent(int start) const;

private:
    void doSearch(int start, GraphVisitor& visitor) const;
    const DirectedWeightedGraph& m_graph;
};

#endif // BFS_H
