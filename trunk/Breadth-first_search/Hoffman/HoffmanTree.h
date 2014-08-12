#ifndef HOFFMANTREE_H
#define HOFFMANTREE_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <istream>

class HoffmanTree
{
public:

    struct Code
    {
        std::bitset<255> data;
        uint8_t size = 0;
    };

    class Node
    {
    public:
        Node(int val = 0) : m_val(val) {}
        uint8_t val() const { return m_val; }
        bool isLeaf() const { return m_node0 == nullptr; }
        Node node0() const { return *m_node0; }
        Node node1() const { return *m_node1; }
        bool is1() const { return m_parent->m_node1 == this; }//является ли node1 у родителя

    private:
        void setNode0(Node* node) { m_node0 = node; node->m_parent = this; }
        void setNode1(Node* node) { m_node1 = node; node->m_parent = this; }

        uint8_t m_val;
        size_t m_quantity = 0;
        Node* m_node0 = nullptr;
        Node* m_node1 = nullptr;
        Node* m_parent = nullptr;

        friend class HoffmanTree;
    };

    using TSerialization = std::vector<uint8_t>;

    static HoffmanTree deserialize(const TSerialization& data);
    static HoffmanTree build(std::istream& stream);

    TSerialization serialize() const;
    Code code(uint8_t val) const;

private:
    Node* m_head = nullptr;

    mutable std::unordered_map<uint8_t, Code> m_codes;
    std::unordered_map<uint8_t, Node*> m_leaves;
};

#endif // HOFFMANTREE_H
