#include "ShortestPathDijkstra.h"

#include <limits>
#include <set>
#include <unordered_set>

ShortestPathDijkstra::ShortestPathDijkstra(const DirectedWeightedGraph& graph, int from, int to)
{
    std::vector<double> dst(graph.vertCount(), std::numeric_limits<double>::infinity());//расстояния от from
    dst[from] = 0.0f;
    std::vector<int> prev(graph.vertCount(), -1);
    std::unordered_set<int> visited;// даёт пространственный выигрыш по сравнению с массивом длины vertCount тольо если ищем один кратчайший путь

    auto lessDst = [&dst](int f, int s) {return dst[f] != dst[s] ? dst[f] < dst[s] : f < s;};
    std::set<int, decltype(lessDst)> verts(lessDst);
    verts.insert(from);

    while(!verts.empty()) // N * log N
    {
        auto i = verts.begin();
        int vert = *i;
        if(vert == to) break;//если целевая вершина здесь, то её расстояние окончательно
        verts.erase(i);
        visited.insert(vert); // C

        auto edgesFromCur = graph.edges(vert);
        for(auto e : edgesFromCur)
        {
            if(visited.count(e.to) != 0) continue; // C
            double thisPathDst = dst[e.from] + e.weight;
            if(thisPathDst < dst[e.to])
            {
                dst[e.to] = thisPathDst;
                verts.insert(e.to);// log N. Заменяем, чтобы найти новое место
                prev[e.to] = e.from;
            }
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
