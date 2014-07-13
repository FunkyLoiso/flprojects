#ifndef PRIMSMINIMUMSPANNINGTREE_H
#define PRIMSMINIMUMSPANNINGTREE_H

#include "Graph.h"

class PrimsMinimumSpanningTree
{
public:
    PrimsMinimumSpanningTree(const DirectedWeightedGraph& graph, int from);
    DirectedWeightedGraph tree() const;
    std::vector<EdgeDsc> edges() const;

private:
    std::vector<EdgeDsc> m_edges;
    int m_numVertices;
};

#endif // PRIMSMINIMUMSPANNINGTREE_H
