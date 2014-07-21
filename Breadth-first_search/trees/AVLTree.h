#ifndef AVLTREE_H
#define AVLTREE_H

#include "BinarySearchTree.h"

#include <algorithm>

template <typename Key, typename Value = Key>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert(const Key &key, const Value &value) override
    {
        std::stack<Node**> insertPath;
        Node** cur = &m_head;
        for(;;)
        {
            Node* node = *cur;
            if(node == nullptr)
            {//добавляем узел, ребалансируем путь
                *cur = new Node{key, value, nullptr, nullptr};
                (*cur)->internalPtr = new int(1);//у листьев высота 1
                rebalance(insertPath);

                return;
            }

            insertPath.push(cur);
            if(key < node->key)
            {
                cur = &(node->left);
            }
            else if(key > node->key)
            {
                cur = &(node->right);
            }
            else
            {// ключи совпадают, заменяем
                node->val = value;
                return;
            }
        }
    }

    // По-видимому обычный вызов перегруженных версий метода из базового класса не работает потому, что класс шаблонный.
    // Альтернативно можно писать AVLTree<int> avl; avl.BinarySearchTree<int>::insert(...);
    using BinarySearchTree::insert;

    virtual bool remove(const Key &key) override
    {
        std::stack<Node**> trace;
        auto tracer = [&trace](Node** n) {trace.push(n);};
        bool found = BinarySearchTree::remove(key, tracer);

        if(found)
        {
            rebalance(trace);
        }

        return found;
    }

private:
    int& height(Node* n)
    {
        return *static_cast<int *>(n->internalPtr);
    }
    int balanceFactor(Node* n)
    {
        int hl = n->left ? height(n->left) : 0;
        int hr = n->right ? height(n->right) : 0;
        return hl - hr;
    }
    void updateHeight(Node* n)
    {
        int hl = n->left ? height(n->left) : 0;
        int hr = n->right ? height(n->right) : 0;
        height(n) = std::max(hl, hr) + 1;
    }

    //      n            b
    //     / \   <-\    / \
    //    l   b   -/   n   r
    //       / \      / \
    //      c   r    l   c
    void rotateLeft(Node*& node)
    {
        Node* n = node;
        node = n->right;   // внешний указатель теперь указывает на b
        n->right = n->right->left;// правый ребёнок n теперь c
        node->left = n;  // левый ребёнок b теперь n

        //запишем новые высоты в n и b. Необходимо ли это?
        updateHeight(node->left);
        updateHeight(node);
    }

    //      n            b
    //     / \   /->    / \
    //    b   r  \-    l   n
    //   / \              / \
    //  l   c            c   r
    void rotateRight(Node*& node)
    {
        Node* n = node;
        node = n->left;   // внешний указатель теперь указывает на b
        n->left = n->left->right;// левый ребёнок n теперь c
        node->right = n;  // правый ребёнок b теперь n

        //запишем новые высоты в n и b. Необходимо ли это?
        updateHeight(node->right);
        updateHeight(node);
    }

    void rebalance(std::stack<Node**>& path)
    {
        while(!path.empty())
        {
            Node** node = path.top();
            path.pop();
            if(*node == nullptr) continue;
            Node* n = *node;


            updateHeight(n);

            if(balanceFactor(n) == 2)
            {//левое поддерево длиннее на 2
                if(balanceFactor(n->left) == -1)
                {//левое поддерево длиннее справа
                    rotateLeft(n->left);
                }
                rotateRight(*node);
            }
            else if(balanceFactor(n) == -2)
            {//правое поддерево дриннее на 2
                if(balanceFactor(n->right) == 1)
                {//правое поддерево длиннее слева
                    rotateRight(n->right);
                }
                rotateLeft(*node);
            }
        }
    }

    void deleteInternalPtr(Node* node)
    {
        delete node->internalPtr;
    }
};

#endif // AVLTREE_H
