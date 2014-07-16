#include "ShortestPathDijkstra.h"

#include <algorithm>
#include <set>

ShortestPathDijkstra::ShortestPathDijkstra(const DirectedWeightedGraph& graph, int from, int to)
{
    std::vector<double> dst(graph.vertCount(), std::numeric_limits<double>::infinity());
    dst[from] = 0.0f;
    std::vector<int> prev(graph.vertCount(), -1);

    std::list<int> verts;
    std::set<int> visited;
    verts.push_back(from);

    auto lessDst = [&dst](int f, int s) {return dst[f] < dst[s];};

    while(!verts.empty())
    {
        auto i = std::min_element(verts.begin(), verts.end(), lessDst);
        int vert = *i;
        if(vert == to) break;//если целевая вершина здесь, то её расстояние окончательно
        verts.erase(i);
        visited.insert(vert);

        auto edgesFromCur = graph.edges(vert);
        for(auto e : edgesFromCur)
        {
            if(visited.count(e.to) != 0) continue;
            if(prev[e.to] == -1) verts.push_back(e.to);//добавляем в список на обработку только если встречаем первый раз

            double thisPathDst = dst[e.from] + e.weight;
            if(thisPathDst < dst[e.to])
            {
                dst[e.to] = thisPathDst;
                prev[e.to] = e.from;
            }
            verts.push_back(e.to);
        }
    }

    m_length = dst[to];
    while(prev[to] != -1)
    {
        m_path.push_front(to);
        to = prev[to];
    }
    if(m_path.size() != 0) m_path.push_front(from);
}
