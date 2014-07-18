#ifndef KRUSKALSMINIMUMSPANNINGTREE_H
#define KRUSKALSMINIMUMSPANNINGTREE_H

#include "Graph.h"

class KruskalsMinimumSpanningTree
{
public:
    KruskalsMinimumSpanningTree(const DirectedWeightedGraph& graph);
    DirectedWeightedGraph tree() const;
    std::vector<EdgeDsc> edges() const;

private:
    std::vector<EdgeDsc> m_edges;
    int m_numVertices;
};

#endif // KRUSKALSMINIMUMSPANNINGTREE_H
