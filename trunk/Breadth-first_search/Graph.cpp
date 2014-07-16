#include "Graph.h"
#include <stdarg.h>
#include <assert.h>
#include <sstream>

DirectedWeightedGraph::DirectedWeightedGraph(int numVertices)
    : m_numVertices(numVertices)
    , m_successors(numVertices)
    , m_weights(numVertices)
{}

void DirectedWeightedGraph::addEdges(const std::vector<EdgeDsc>& edges)
{
    for(auto e : edges)
	{
        assert(e.from < m_numVertices);
        assert(e.to < m_numVertices);

        addEdge(e.from, e.to, e.weight);
	}
}

void DirectedWeightedGraph::addPath(const std::vector<int>& verts, const std::vector<double>& weights)
{
    assert(verts.size() > 1);
    assert(weights.size() == verts.size()-1);

    int from = verts.at(0);
    for(std::size_t i = 1; i < verts.size(); ++i)
    {
        int to = verts.at(i);

        assert(from < m_numVertices);
        assert(to < m_numVertices);

        addEdge(from, to, weights[i-1]);
        from = to;
    }
}

int DirectedWeightedGraph::vertCount() const
{
    return m_numVertices;
}

DirectedWeightedGraph DirectedWeightedGraph::inverted() const
{
    DirectedWeightedGraph result(m_numVertices);
    auto addInverted = [&result](const EdgeDsc& e) {result.addEdge(e.to, e.from, e.weight);};
    forAllEdges(addInverted);
    return result;
}

const std::vector<int>& DirectedWeightedGraph::successors(int vert) const
{
    assert(vert < m_numVertices);
    return m_successors[vert];
}

std::vector<EdgeDsc> DirectedWeightedGraph::edges(int from) const
{
    assert(from < m_numVertices);

    const auto succs = m_successors.at(from);
    const auto weights = m_weights.at(from);
    std::vector<EdgeDsc> result(succs.size());

    for(std::size_t i = 0; i < succs.size(); ++i)
    {
        result[i] = {from, succs.at(i), weights.at(i)};
    }
    return result;
}

std::vector<EdgeDsc> DirectedWeightedGraph::allEdges() const
{
    std::vector<EdgeDsc> result;
    auto addToResult = [&result](const EdgeDsc& e) {result.push_back(e);};
    forAllEdges(addToResult);
    return result;
}

std::string DirectedWeightedGraph::toString() const
{
    std::ostringstream str;

    str << "Neighbours table:" << std::endl;
    for(int vert = 0; vert < m_numVertices; ++vert)
    {
        str << vert << ": ";
        const auto succs = m_successors.at(vert);
        const auto weights = m_weights.at(vert);
        for(std::size_t succ = 0; succ < succs.size(); ++succ)
        {
            str << succs.at(succ) << '(' << weights.at(succ) << ") ";
        }
        str << std::endl;
    }
    str << std::endl;

    return str.str();
}

void DirectedWeightedGraph::addEdge(int from, int to, double weight)
{
    m_successors.at(from).push_back(to);
    m_weights.at(from).push_back(weight);
}

void DirectedWeightedGraph::forAllEdges(std::function<void(const EdgeDsc&)> func) const
{
    for(int vert = 0; vert < m_numVertices; ++vert)
    {
        const auto succs = m_successors.at(vert);
        const auto weights = m_weights.at(vert);
        for(size_t s = 0; s < succs.size(); ++s)
        {
            func({ vert, succs.at(s), weights.at(s) });
        }
    }
}
