#include <iostream>
#include "Graph.h"
#include "bfs.h"

template<typename C>
void printCollection(const C& coll)
{
    if(coll.size() == 0) std::cout << "<empty>";
    else
    {
        for(auto i = coll.cbegin(); i != coll.cend(); ++i)
        {
            std::cout << *i << ", ";
        }
    }
    std::cout << std::endl;
}

std::tuple<int, int> var {1,2};
std::vector<std::tuple<int, int>> vev {std::make_tuple(1,2), std::make_tuple(2,1) };

int main()
{
    DirectedWeightedGraph g(10);
    //g.addPath(5, 0, 1, 2, 0, 9, 0);
    g.addEdges({ {1, 2, 0.5}, {3, 4, -0.7} });
    g.addPath( {3, 4, 5, 6, 7}, {1.0, 1.0, 1.0, 1.0} );
    g.addPath( {2, 4, 1, 7}, {0.5, 0.5, 0.5} );
    std::cout << g.toString();

    BFS bfs(g);

    std::cout << "connected component from 0: ";
    printCollection(bfs.connectedComponent(0));
    std::cout << "connected component from 5: ";
    printCollection(bfs.connectedComponent(5));
    std::cout << "connected component from 8: ";
    printCollection(bfs.connectedComponent(8));

    std::cout << "shortest path 0 -> 9 is: ";
    printCollection(bfs.shortestPath(0, 9));
    std::cout << "shortest path 4 -> 7 is: ";
    printCollection(bfs.shortestPath(4, 7));
    std::cout << "shortest path 5 -> 4 is: ";
    printCollection(bfs.shortestPath(5, 4));
    std::cout << "shortest path 6 -> 4 is: ";
    printCollection(bfs.shortestPath(6, 4));

	return 0;
}
