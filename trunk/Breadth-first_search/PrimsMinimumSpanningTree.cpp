#include "PrimsMinimumSpanningTree.h"

#include <limits>
#include <set>

PrimsMinimumSpanningTree::PrimsMinimumSpanningTree(const DirectedWeightedGraph &graph, int from)
    : m_numVertices(graph.vertCount())
{
    std::vector<double> dst(graph.vertCount(), std::numeric_limits<double>::infinity());
    dst[from] = 0.0f;
    std::vector<int> prev(graph.vertCount(), -1);
    std::vector<bool> visited(graph.vertCount(), false);

    auto lessDst = [&dst](int f, int s) {return dst[f] != dst[s] ? dst[f] < dst[s] : f < s;};
    std::set<int, decltype(lessDst)> verts(lessDst);
    verts.insert(from);

    while(!verts.empty()) // N * log N
    {
        auto i = verts.begin();
        int vert = *i;
        verts.erase(i);
        visited[vert] = true;
        if(vert != from)//грань до первой вершины добавлять не надо
        {
            m_edges.push_back({ prev[vert], vert, dst[vert] });
        }

        auto edgesFromCur = graph.edges(vert);
        for(auto e : edgesFromCur)
        {
            if(visited.at(e.to) == true) continue;
            if(e.weight < dst[e.to])
            {
                dst[e.to] = e.weight;
                verts.insert(e.to);// log N. Заменяем, чтобы найти новое место
                prev[e.to] = e.from;
            }
        }
    }
}

DirectedWeightedGraph PrimsMinimumSpanningTree::tree() const
{
    DirectedWeightedGraph graph(m_numVertices);
    graph.addEdges(m_edges);
    return graph;
}

std::vector<EdgeDsc> PrimsMinimumSpanningTree::edges() const
{
    return m_edges;
}
