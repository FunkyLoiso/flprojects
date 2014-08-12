#include "HoffmanTree.h"

#include <numeric>
#include <algorithm>

HoffmanTree HoffmanTree::build(std::istream& stream)
{
    std::vector<Node*> nodes(256);
//    std::iota(nodes.begin(), nodes.end(), 0);
    for(int i = 0; i < 256; ++i)
    {
        nodes.at(i) = new Node(i);
    }

    //1. Просканируем поток и посчитаем число появлений каждого байта
    while(stream.good())
    {
        uint8_t byte = stream.get();
        if(stream.good()) ++nodes.at(byte)->m_quantity;
    }

    //2. Упорядочим ноды в кучу по возрастанию числа появления
    auto higherQuantity = [](Node* f, Node* s) { return f->m_quantity > s->m_quantity; };
    std::make_heap(nodes.begin(), nodes.end(), higherQuantity);

    //3. Создадим дерево снизу вверх, соединяя самые редкие ноды одним родителем. За ожно соберём адреса листьев в map
    std::unordered_map<uint8_t, Node*> leaves;
    while(nodes.size() > 1)
    {
        Node* n1 = nodes.front();
        std::pop_heap(nodes.begin(), nodes.end(), higherQuantity), nodes.pop_back();
        if(n1->m_quantity == 0)//пропустим значения, которые не встречаются вообще
        {
            delete n1;
            continue;
        }
        Node* n2 = nodes.front();
        std::pop_heap(nodes.begin(), nodes.end(), higherQuantity), nodes.pop_back();

        Node* p = new Node;
        p->m_quantity = n1->m_quantity + n2->m_quantity;
        p->setNode0(n1);
        p->setNode1(n2);

        nodes.push_back(p);
        std::push_heap(nodes.begin(), nodes.end(), higherQuantity);

        if(n1->isLeaf()) leaves[n1->val()] = n1;
        if(n2->isLeaf()) leaves[n2->val()] = n2;
    }

    //4. В куче остался только один нод, это голова дерева (если, конечно, его quantity не равно 0)

    HoffmanTree result;

    Node* last = nodes.front();
    if(last->m_quantity != 0)
    {
        result.m_head = last;
        if(last->isLeaf()) leaves[last->val()] = last;
    }
    else
    {
        delete last;
        result.m_head = nullptr;
    }

    result.m_leaves = std::move(leaves);
    return result;
}

HoffmanTree HoffmanTree::deserialize(const HoffmanTree::TSerialization &data)
{
    return HoffmanTree();
}

HoffmanTree::TSerialization HoffmanTree::serialize() const
{
    return TSerialization();
}

HoffmanTree::Code HoffmanTree::code(uint8_t val) const
{
    //лениво подсчитаем код для значения
    auto i = m_codes.find(val);
    if(i != m_codes.end())
    {
        return i->second;
    }
    else
    {
        Code c;
        auto leafI = m_leaves.find(val);
        if(leafI != m_leaves.end())
        {
            Node* n = leafI->second;
            auto place = 0;
            while(n->m_parent != nullptr)
            {
                c.data.set(place++, n->is1());
                n = n->m_parent;
            }
            c.size = place;
        }
        m_codes[val] = c;
        return c;
    }
}

