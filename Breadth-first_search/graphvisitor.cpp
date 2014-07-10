#include "graphvisitor.h"

#include <limits>

void ConnectedComponentVisitor::visit(int parent, int child)
{
    m_result.insert(parent);
    m_result.insert(child);
}

bool ConnectedComponentVisitor::done()
{
    return false;
}

std::set<int> ConnectedComponentVisitor::getResult() const
{
    return m_result;
}


//std::vector<int> dst(m_graph.vertCount(), std::numeric_limits<int>::max());
//std::vector<int> prev(m_graph.vertCount(), -1);

//std::queue<int> queue;
//std::set<int> visited;
//queue.push(from);
//dst[from] = 0;

//while(!queue.empty())
//{
//    int vert = queue.front();
//    if(vert == to) break;
//    queue.pop();
//    visited.insert(vert);

//    auto neighbours = m_graph.neighbours(vert);
//    for(auto n = neighbours.begin(); n != neighbours.end(); ++n)
//    {
//        if(visited.count(*n) == 0)
//        {
//            int thisPathDst = dst[vert] + 1;
//            if(thisPathDst < dst[*n])
//            {
//                dst[*n] = thisPathDst;
//                prev[*n] = vert;
//            }
//            queue.push(*n);
//        }
//    }
//}

//std::list<int> path;
//while(prev[to] != -1)
//{
//    path.push_front(to);
//    to = prev[to];
//}
//if(path.size() != 0) path.push_front(from);

//return path;


ShortestPathVisitor::ShortestPathVisitor(int numVertices, int from, int to)
    : m_dst(numVertices, std::numeric_limits<int>::max())
    , m_prev(numVertices, -1)
    , m_from(from)
    , m_to(to)
{

}

void ShortestPathVisitor::visit(int parent, int child)
{
    int thisPathDst = m_dst[parent] + 1;
    if(thisPathDst < m_dst[child])
    {
        m_dst[child] = thisPathDst;
        m_prev[child] = parent;
    }

    if(child == m_to) m_done = true;
}

bool ShortestPathVisitor::done()
{
    return m_done;
}

std::list<int> ShortestPathVisitor::getResult() const
{
    int to = m_to;
    std::list<int> path;
    while(m_prev[to] != -1)
    {
        path.push_front(to);
        to = m_prev[to];
    }
    if(path.size() != 0) path.push_front(m_from);

    return path;
}
