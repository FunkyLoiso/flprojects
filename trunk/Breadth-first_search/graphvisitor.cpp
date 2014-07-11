#include "graphvisitor.h"

#include <limits>

void ConnectedComponentVisitor::visit(const EdgeDsc &edge)
{
    m_result.insert(edge.to);
}

bool ConnectedComponentVisitor::done()
{
    return false;
}

std::set<int> ConnectedComponentVisitor::getResult() const
{
    return m_result;
}
