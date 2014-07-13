#ifndef GRAPHVISITOR_H
#define GRAPHVISITOR_H

#include <set>
#include <list>
#include <vector>

#include "Graph.h"

class GraphVisitor
{
public:
    virtual void visit(const EdgeDsc &edge) = 0;
    virtual bool done() = 0;
};

class ConnectedComponentVisitor : public GraphVisitor
{
public:
    void visit(const EdgeDsc &edge);
    bool done();

    std::set<int> getResult() const;

private:
    std::set<int> m_result;
};

#endif // GRAPHVISITOR_H
