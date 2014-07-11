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
