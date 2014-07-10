#ifndef GRAPHVISITOR_H
#define GRAPHVISITOR_H

#include <set>
#include <list>
#include <vector>

class GraphVisitor
{
public:
    virtual void visit(int parent, int child) = 0;
    virtual bool done() = 0;
};

class ConnectedComponentVisitor : public GraphVisitor
{
public:
    void visit(int parent, int child);
    bool done();

    std::set<int> getResult() const;

private:
    std::set<int> m_result;
};

class ShortestPathVisitor : public GraphVisitor
{
public:
    ShortestPathVisitor(int numVertices, int from, int to);

    void visit(int parent, int child);
    bool done();

    std::list<int> getResult() const;

private:
    int m_from, m_to;
    bool m_done {false};
    std::vector<int> m_dst;
    std::vector<int> m_prev;
};

#endif // GRAPHVISITOR_H
