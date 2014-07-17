#include "KruskalsMinimumSpanningTree.h"

#include <algorithm>
#include <unordered_set>
#include "DisjointSet.h"

KruskalsMinimumSpanningTree::KruskalsMinimumSpanningTree(const DirectedWeightedGraph& graph)
    : m_numVertices(graph.vertCount())
{
    std::vector<EdgeDsc> edges = graph.allEdges();
    auto lessWeight = [](const EdgeDsc& f, const EdgeDsc& s) { return f.weight < s.weight; };
    std::sort(edges.begin(), edges.end(), lessWeight);

    DisjointSet ds(m_numVertices);
    std::unordered_set<int> visitedVertices;
    for(auto e : edges) // N
    {
        if(ds.find(e.from) != ds.find(e.to)) // C
        {
            m_edges.push_back(e);
            visitedVertices.insert(e.from); // C
            visitedVertices.insert(e.to);   // C
            if( visitedVertices.size() == static_cast<std::size_t>(graph.vertCount()) ) return;

            ds.join(e.from, e.to); // C
        }
    }
}

DirectedWeightedGraph KruskalsMinimumSpanningTree::tree() const
{
    DirectedWeightedGraph graph(m_numVertices);
    graph.addEdges(m_edges);
    return graph;
}

std::vector<EdgeDsc> KruskalsMinimumSpanningTree::edges() const
{
    return m_edges;
}
