#include "KosarajusSCCs.h"

#include <set>
#include <stack>

KosarajusSCCs::KosarajusSCCs(const DirectedWeightedGraph& graph)
{
    std::set<int> visited;
    std::stack<int> finishOrder;

    //перый проход DFS
    for(int vert = 0; vert < graph.vertCount(); ++vert)
    {
        if(visited.count(vert) != 0) continue;
        std::stack<int> dfsStack;
        dfsStack.push(vert);
        while (!dfsStack.empty())
        {
            int dfsVert = dfsStack.top();//пока не извлекаем из стека
            visited.insert(dfsVert);

            bool finished = true;
            for(const EdgeDsc& e : graph.edges(dfsVert))
            {
                if(visited.count(e.to) != 0) continue;
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

    //второй проход DFS
    visited.clear();
    DirectedWeightedGraph inv = graph.inverted();
    while (!finishOrder.empty())
    {
        int vert = finishOrder.top();
        finishOrder.pop();

        if(visited.count(vert) != 0) continue;

        std::vector<int> scc;
//        scc.push_back(vert);
        std::stack<int> dfsStack;
        dfsStack.push(vert);
        while(!dfsStack.empty())
        {
            int dfsVert = dfsStack.top();
            dfsStack.pop();
            if(visited.count(dfsVert) != 0) continue;
            visited.insert(dfsVert);
            scc.push_back(dfsVert);//добавим вершину к сильно связанному компоненту

            for(const EdgeDsc& e : inv.edges(dfsVert))
            {
                if(visited.count(e.to) != 0) continue;
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
