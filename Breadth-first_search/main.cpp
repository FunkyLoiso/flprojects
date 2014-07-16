#include <iostream>
#include "Graph.h"
#include "BFS.h"
#include "ShortestPathDijkstra.h"
#include "PrimsMinimumSpanningTree.h"
#include "KruskalsMinimumSpanningTree.h"
#include "KosarajusSCCs.h"

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
}

std::ostream& operator<<(std::ostream& stream, const EdgeDsc& e)
{
    return stream << e.from << "->" << e.to << "(" << e.weight << ")";
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

    //минимальное дерево по Приму
    std::cout << "Min spanning tree from 4:" << std::endl << PrimsMinimumSpanningTree(g, 4).tree().toString() << std::endl;
    std::cout << "Min spanning tree from 5:" << std::endl << PrimsMinimumSpanningTree(g, 5).tree().toString() << std::endl;
    std::cout << "Min spanning tree from 6:" << std::endl << PrimsMinimumSpanningTree(g, 6).tree().toString() << std::endl;

    //минимальное дерево по Краскалу
    std::cout << "Min spanning tree (Kruskal):" << std::endl << KruskalsMinimumSpanningTree(g).tree().toString() << std::endl;

    //сильно связанные компоненты Косарайю
    DirectedWeightedGraph g2(10);
    g2.addPath({1, 7, 9, 6, 8, 2, 5, 8}, {1, 1, 1, 1, 1, 1, 1});
    g2.addPath({7, 4, 1}, {1, 1});
    g2.addPath({6, 3, 9}, {1, 1});

    std::cout << "g2:" << std::endl << g2.toString() << std::endl;
    std::cout << "SCCs of g2 :" << std::endl;
    KosarajusSCCs sccs(g2);
    for(auto scc : sccs.SCCs())
    {
        static int counter = 0;
        std::cout << counter++ << ": ";
        printCollection(scc);
        std::cout << std::endl;
    }

    //
    return 0;

}
