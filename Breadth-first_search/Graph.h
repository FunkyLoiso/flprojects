#ifndef Graph_h__
#define Graph_h__

#include <vector>
#include <string>

class UndirectedGraph
{
public:
    UndirectedGraph(int numVertices);
	void addEdges(int numPairs, ...);
	void addPath(int length, ...);
    int vertCount() const;

    const std::vector<int>& neighbours(int vert) const;
    std::string toString() const;

private:
    std::vector<std::vector<int> > m_neighbours;
	int m_numVertices;
};

#endif // Graph_h__
