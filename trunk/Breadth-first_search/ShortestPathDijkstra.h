#ifndef SHORTESTPATHDIJKSTRA_H
#define SHORTESTPATHDIJKSTRA_H

#include "Graph.h"

class ShortestPathDijkstra
{
public:
    ShortestPathDijkstra(const DirectedWeightedGraph& graph, int from, int to);
    std::list<int> path() const {return m_path;}
    double length() const {return m_length;}

private:
    std::list<int> m_path;
    double m_length;
};

#endif // SHORTESTPATHDIJKSTRA_H
