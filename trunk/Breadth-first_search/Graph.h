#ifndef Graph_h__
#define Graph_h__

#include <vector>
#include <list>
#include <tuple>
#include <string>

struct EdgeDsc
{
    int from, to;
    double weight;

};

class DirectedWeightedGraph
{
public:
    DirectedWeightedGraph(int numVertices);
    void addEdges(std::vector<EdgeDsc>&& edges);
    void addPath(std::vector<int>&& verts, std::vector<double>&& weights);
    int vertCount() const;

    const std::vector<int>& successors(int vert) const; // вершины, до которых есть ребро
    std::vector<EdgeDsc> edges(int from) const; // рёбра из вершины

    std::string toString() const;

private:
    std::vector<std::vector<int>> m_successors;
    std::vector<std::vector<double>> m_weights;
	int m_numVertices;

    void addEdge(int from, int to, double weight);
};

#endif // Graph_h__
