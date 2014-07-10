#include "bfs.h"
#include <queue>

BFS::BFS(const UndirectedGraph &graph) : m_graph(graph)
{
}

std::set<int> BFS::connectedComponent(int start) const
{
    ConnectedComponentVisitor v;
    doSearch(start, v);
    return v.getResult();
}

std::list<int> BFS::shortestPath(int from, int to) const
{
    ShortestPathVisitor v(m_graph.vertCount(), from, to);
    doSearch(from, v);
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

        auto neighbours = m_graph.neighbours(vert);
        for(auto n = neighbours.begin(); n != neighbours.end(); ++n)
        {
            if(visited.count(*n) == 0)
            {
                visitor.visit(vert, *n);
                queue.push(*n);
            }
        }
    }
}
