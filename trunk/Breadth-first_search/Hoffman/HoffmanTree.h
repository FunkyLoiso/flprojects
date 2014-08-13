#ifndef HOFFMANTREE_H
#define HOFFMANTREE_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <istream>

struct HoffmanCode
{
    using TData = std::bitset<255>;
    TData data;
    uint8_t size = 0;
};


class HoffmanTree
{
public:

    static HoffmanTree build(std::istream& stream);

//    HoffmanCode code(uint8_t val) const;
//    std::vector<HoffmanCode> codes() const;

    class Node
    {
    public:
        Node(int val = 0) : val(val) {}
        bool isLeaf() const { return node0 == nullptr; }
        bool is1() const { return parent->node1 == this; }//является ли node1 у родителя

        void setNode0(Node* node) { node0 = node; node->parent = this; }
        void setNode1(Node* node) { node1 = node; node->parent = this; }

        uint8_t val;
        size_t quantity = 0;
        Node* node0 = nullptr;
        Node* node1 = nullptr;
        Node* parent = nullptr;
    }* head = nullptr;

//    mutable std::unordered_map<uint8_t, HoffmanCode> m_codes;
    std::unordered_map<uint8_t, Node*> leaves;
};

#endif // HOFFMANTREE_H
