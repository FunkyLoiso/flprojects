#include "kruskalsminimumspanningtree.h"

#include <algorithm>
#include <set>
#include "DisjointSet.h"

KruskalsMinimumSpanningTree::KruskalsMinimumSpanningTree(const DirectedWeightedGraph& graph)
    : m_numVertices(graph.vertCount())
{
    struct
    {
        bool operator()(const EdgeDsc& f, const EdgeDsc& s) { return f.weight < s.weight; }
    }lessWeight;

    std::vector<EdgeDsc> edges = graph.allEdges();
    std::sort(edges.begin(), edges.end(), lessWeight);

    DisjointSet ds(m_numVertices);
    std::set<int> visitedVertices;
    for(auto e : edges)
    {
        if(ds.find(e.from) != ds.find(e.to))
        {
            m_edges.push_back(e);
            visitedVertices.insert(e.from);
            visitedVertices.insert(e.to);
            if(visitedVertices.size() == graph.vertCount()) return;

            ds.join(e.from, e.to);
        }
    }
}

DirectedWeightedGraph KruskalsMinimumSpanningTree::tree() const
{
    DirectedWeightedGraph graph(m_numVertices);
    graph.addEdges(std::move(m_edges));
    return graph;
}

std::vector<EdgeDsc> KruskalsMinimumSpanningTree::edges() const
{
    return m_edges;
}
