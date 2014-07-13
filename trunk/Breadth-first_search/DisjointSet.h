#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include <vector>

class DisjointSet
{
public:
    DisjointSet(int count) : m_parent(count), m_rank(count, 0)
    {
        for(int i = 0; i < count; ++i) m_parent[i] = i;
    }
    int find(int x) const
    {
        //return ( x == m_parent[x] ? x : m_parent[x] = find(m_parent[x]) );
        // заменим рекурсию итерацией

        if(x == m_parent[x]) return x;

        //найдём корень дерева, попутно перенося все листся к корню
        std::vector<int> treeElements;
        do
        {
            treeElements.push_back(x);
            x = m_parent[x];
        }
        while(x != m_parent[x]);//у корневрго элемента родитель равняется самому элементу

        for(int e : treeElements)
        {
            m_parent[e] = x;
        }
        return x;
    }

    void join(int x, int y)
    {
        x = find(x);
        y = find(y);

        if(x == y) return;//и так одно множество

        //помешаем меньшее дерево под корневой элемент большего
        if(m_rank[x] < m_rank[y]) m_parent[x] = y;
        else m_parent[y] = x;
        //дерево углубляется только если ранки равны, в таком случае помещаем в первое дерево и увеличиваем его ранк
        if(m_rank[x] == m_rank[y]) ++m_rank[x];
    }

private:
    mutable std::vector<int> m_parent;//элемент-родитель
    std::vector<int> m_rank;//ранк дерева с корневым элементом
};

#endif // DISJOINTSET_H
