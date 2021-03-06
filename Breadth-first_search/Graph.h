#ifndef Graph_h__
#define Graph_h__

#include <vector>
#include <list>
#include <tuple>
#include <string>
#include <functional>

struct EdgeDsc
{
    int from, to;
    double weight;
};

class DirectedWeightedGraph
{
public:
    DirectedWeightedGraph(int numVertices);

    void addEdges(const std::vector<EdgeDsc>& edges);
    void addPath(const std::vector<int>& verts, const std::vector<double>& weights);
    int vertCount() const;
    DirectedWeightedGraph inverted() const;

    const std::vector<int>& successors(int vert) const; // вершины, до которых есть ребро
    std::vector<EdgeDsc> edges(int from) const; // рёбра из вершины
    std::vector<EdgeDsc> allEdges() const;  // все рёбра

    std::string toString() const;

private:
    int m_numVertices;
    std::vector<std::vector<int>> m_successors;
    std::vector<std::vector<double>> m_weights;

    void addEdge(const EdgeDsc& edge);
    void forAllEdges(std::function<void(const EdgeDsc&)>) const;
};

#endif // Graph_h__
