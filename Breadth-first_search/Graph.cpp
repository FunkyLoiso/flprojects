#include "Graph.h"
#include <stdarg.h>
#include <assert.h>
#include <sstream>

UndirectedGraph::UndirectedGraph(int numVertices) : m_numVertices(numVertices)
{
	for(int i = 0; i < m_numVertices; ++i)
	{
        m_neighbours.push_back(std::vector<int>());
	}
}

void UndirectedGraph::addEdges(int numPairs, ...)
{
	va_list ap;
	va_start(ap, numPairs);
    for(int i = 0; i < numPairs; ++i)
	{
		int from = va_arg(ap, int);
		int to = va_arg(ap, int);

        assert(from < m_numVertices);
        assert(to < m_numVertices);

        m_neighbours[from].push_back(to);
        m_neighbours[to].push_back(from);
	}
	va_end(ap);
}

void UndirectedGraph::addPath(int length, ...)
{
    va_list ap;
    va_start(ap, length);
    int from = va_arg(ap, int);
    for(int i = 0; i < length; ++i)
    {
        int to = va_arg(ap, int);

        assert(from < m_numVertices);
        assert(to < m_numVertices);

        m_neighbours[from].push_back(to);
        m_neighbours[to].push_back(from);
        from = to;
    }
    va_end(ap);
}

int UndirectedGraph::vertCount() const
{
    return m_numVertices;
}

const std::vector<int>& UndirectedGraph::neighbours(int vert) const
{
    assert(vert < m_numVertices);
    return m_neighbours[vert];
}

std::string UndirectedGraph::toString() const
{
    std::ostringstream str;

    str << "Neighbours table:" << std::endl;
    for(int vert = 0; vert < m_numVertices; ++vert)
    {
        str << vert << ": ";
        const std::vector<int> succs = m_neighbours[vert];
        for(int succ = 0; succ < succs.size(); ++succ)
        {
            str << succs[succ] << ' ';
        }
        str << std::endl;
    }
    str << std::endl;

	return str.str();
}
