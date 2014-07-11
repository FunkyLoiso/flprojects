#include "bfs.h"
#include <queue>

BFS::BFS(const DirectedWeightedGraph &graph) : m_graph(graph)
{
}

std::set<int> BFS::reachableComponent(int start) const
{
    ConnectedComponentVisitor v;
    doSearch(start, v);
    return v.getResult();
}

void BFS::doSearch(int start, GraphVisitor &visitor) const
{
    std::queue<int> queue;
    std::set<int> visited;
    queue.push(start);

    while(!queue.empty() && !visitor.done())
    {
        int vert = queue.front();
        queue.pop();
        visited.insert(vert);

        auto edges = m_graph.edges(vert);
        for(auto e : edges)
        {
            if(visited.count(e.to) == 0)
            {
                visitor.visit(e);
                queue.push(e.to);
            }
        }
    }
}
