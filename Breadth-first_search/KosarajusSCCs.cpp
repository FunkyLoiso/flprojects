#include "KosarajusSCCs.h"

#include <set>
#include <stack>

#include <cstring>

KosarajusSCCs::KosarajusSCCs(const DirectedWeightedGraph& graph)
{
    std::vector<bool> visited(graph.vertCount(), false);
    std::stack<int> finishOrder;

    //перый проход DFS
    for(int vert = 0; vert < graph.vertCount(); ++vert) // N
    {
        if(visited[vert]) continue;
        std::stack<int> dfsStack;
        dfsStack.push(vert);
        while (!dfsStack.empty())
        {
            int dfsVert = dfsStack.top();//пока не извлекаем из стека
            visited[dfsVert] = true;

            bool finished = true;
            for(const EdgeDsc& e : graph.edges(dfsVert))
            {
                if(visited[e.to]) continue;
                dfsStack.push(e.to);
                finished = false;
            }
            if(finished)
            {//если дети уже обработаны
                dfsStack.pop();
                finishOrder.push(dfsVert);
            }
        }
    }

    std::fill(visited.begin(), visited.end(), false);// N
    //второй проход DFS
    DirectedWeightedGraph inv = graph.inverted();// N
    while (!finishOrder.empty()) // N
    {
        int vert = finishOrder.top();
        finishOrder.pop();

        if(visited[vert]) continue;

        std::vector<int> scc;
        std::stack<int> dfsStack;
        dfsStack.push(vert);
        while(!dfsStack.empty())
        {
            int dfsVert = dfsStack.top();
            dfsStack.pop();
            if(visited[dfsVert]) continue;
            visited[dfsVert] = true;
            scc.push_back(dfsVert);//добавим вершину к сильно связанному компоненту

            for(const EdgeDsc& e : inv.edges(dfsVert))
            {
                if(visited[e.to]) continue;
                dfsStack.push(e.to);
            }
        }
        m_sccs.push_back(scc);
    }
}


std::list<std::vector<int>> KosarajusSCCs::SCCs() const
{
    return m_sccs;
}
