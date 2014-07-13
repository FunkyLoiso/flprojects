#include <iostream>
#include "Graph.h"
#include "bfs.h"
#include "shortestpathdijkstra.h"
#include "primsminimumspanningtree.h"

#include "DisjointSet.h"

template<typename C>
void printCollection(const C& coll)
{
    if(coll.size() == 0) std::cout << "<empty>";
    else
    {
        for(auto val : coll)
        {
            std::cout << val << ", ";
        }
    }
//    std::cout << std::endl;
}

std::tuple<int, int> var {1,2};
std::vector<std::tuple<int, int>> vev {std::make_tuple(1,2), std::make_tuple(2,1) };

int main()
{
    DirectedWeightedGraph g(10);
    //g.addPath(5, 0, 1, 2, 0, 9, 0);
    g.addEdges({ {1, 2, 0.5}, {3, 4, -0.7} });
    g.addPath( {3, 4, 5, 6, 7}, {0.1, 0.1, 0.1, 0.1} );
    g.addPath( {7, 6, 5, 4, 3}, {1.0, 1.0, 1.0, 1.0} );
    g.addPath( {2, 4, 1, 7}, {0.5, 0.5, 0.5} );
    g.addPath( {7, 1, 4, 2}, {0.5, 0.5, 0.5} );
    std::cout << g.toString();

    //достижимый подграф
    BFS bfs(g);

    std::cout << "reachable component from 4: ";
    printCollection(bfs.reachableComponent(4));
    std::cout << "\nreachable component from 5: ";
    printCollection(bfs.reachableComponent(5));
    std::cout << "\nreachable component from 6: ";
    printCollection(bfs.reachableComponent(6));

    //кратчайший путь
    auto p = ShortestPathDijkstra(g, 0, 9);
    std::cout << "\nshortest path 0 -> 9 is: ";
    printCollection(p.path());
    std::cout << "length: " << p.length() << std::endl;

    p = ShortestPathDijkstra(g, 4, 7);
    std::cout << "shortest path 4 -> 7 is: ";
    printCollection(p.path());
    std::cout << "length: " << p.length() << std::endl;

    p = ShortestPathDijkstra(g, 5, 4);
    std::cout << "shortest path 5 -> 4 is: ";
    printCollection(p.path());
    std::cout << "length: " << p.length() << std::endl;

    p = ShortestPathDijkstra(g, 6, 4);
    std::cout << "shortest path 6 -> 4 is: ";
    printCollection(p.path());
    std::cout << "length: " << p.length() << std::endl;

    //минимальное дерево
    std::cout << "Min spanning tree from 4:" << std::endl << PrimsMinimumSpanningTree(g, 4).tree().toString() << std::endl;
    std::cout << "Min spanning tree from 5:" << std::endl << PrimsMinimumSpanningTree(g, 5).tree().toString() << std::endl;
    std::cout << "Min spanning tree from 6:" << std::endl << PrimsMinimumSpanningTree(g, 6).tree().toString() << std::endl;

    DisjointSet ds(3);
    std::cout << ds.find(0) << ' ' << ds.find(1) << ' ' << ds.find(2) << std::endl;
    ds.join(1, 2);
    std::cout << ds.find(0) << ' ' << ds.find(1) << ' ' << ds.find(2) << std::endl;
    ds.join(3, 1);
    std::cout << ds.find(0) << ' ' << ds.find(1) << ' ' << ds.find(2) << std::endl;

    //
    return 0;

}
