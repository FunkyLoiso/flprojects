#include "PrimsMinimumSpanningTree.h"

#include <algorithm>
#include <set>

PrimsMinimumSpanningTree::PrimsMinimumSpanningTree(const DirectedWeightedGraph &graph, int from)
    : m_numVertices(graph.vertCount())
{
    std::vector<double> dst(graph.vertCount(), std::numeric_limits<double>::infinity());
    dst[from] = 0.0f;
    std::vector<int> prev(graph.vertCount(), -1);

    std::list<int> verts;
    std::set<int> visited;
    verts.push_back(from);  //добавим начальную вершину

    auto lessDst = [&dst](int f, int s) {return dst[f] < dst[s];};

    while(!verts.empty())
    {
        auto i = std::min_element(verts.begin(), verts.end(), lessDst);
        int vert = *i;
        verts.erase(i);
        visited.insert(vert);
        if(vert != from)//грань до первой вершины добавлять не надо
        {
            m_edges.push_back({ prev[vert], vert, dst[vert] });
        }

        auto edgesFromCur = graph.edges(vert);
        for(auto e : edgesFromCur)
        {
            if(visited.count(e.to) != 0) continue;
            if(prev[e.to] == -1) verts.push_back(e.to);//добавляем в список на обработку только если встречаем первый раз

            if(e.weight < dst[e.to])
            {
                dst[e.to] = e.weight;
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
