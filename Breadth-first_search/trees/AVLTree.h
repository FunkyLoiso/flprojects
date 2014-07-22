#ifndef AVLTREE_H
#define AVLTREE_H

#include "BinarySearchTree.h"

#include <algorithm>

template <typename Key, typename Value = Key>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:

    // По-видимому обычный вызов перегруженных версий метода из базового класса не работает потому, что класс шаблонный.
    // Альтернативно можно писать AVLTree<int> avl; avl.BinarySearchTree<int>::insert(...);
//    using BinarySearchTree::insert;

private:
    /*
     * Дополнительная информация для узлов
     */
    struct info { Node* parent; int height; };

    Node*& parent(Node* n)
    {
        return static_cast<info *>(n->internalPtr)->parent;
    }
    // получить ссылку на поле родителя, в поторой записан адрес n
    Node*& parentPtrTo(Node* n)
    {
        Node* p = parent(n);
        if(p == nullptr) return m_head;
        if(p->left == n) return p->left;
        else return p->right;
    }
    int& height(Node* n)
    {
        return static_cast<info *>(n->internalPtr)->height;
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

    /*
     * Повороты
     */

    //      n            b
    //     / \   <-\    / \
    //    l   b   -/   n   r
    //       / \      / \
    //      c   r    l   c
    void rotateLeft(Node*& node)
    {
        Node* n = node;
        node = n->right;   // внешний указатель теперь указывает на b
        parent(node) = parent(n);

        n->right = n->right->left;// правый ребёнок n теперь c
        if(n->right) parent(n->right) = n;//c может не быть

        node->left = n;  // левый ребёнок b теперь n
        parent(node->left) = node;

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
        parent(node) = parent(n);

        n->left = n->left->right;// левый ребёнок n теперь c
        if(n->left) parent(n->left) = n;//с может не быть

        node->right = n;  // правый ребёнок b теперь n
        parent(node->right) = node;

        //запишем новые высоты в n и b. Необходимо ли это?
        updateHeight(node->right);
        updateHeight(node);
    }

    /*
     * Ребалансировка
     */
    void rebalance(Node* node)
    {
        while(node != nullptr)
        {
            Node*& n = parentPtrTo(node);

            updateHeight(n);

            if(balanceFactor(n) == 2)
            {//левое поддерево длиннее на 2
                if(balanceFactor(n->left) == -1)
                {//левое поддерево длиннее справа
                    rotateLeft(n->left);
                }
                rotateRight(n);
            }
            else if(balanceFactor(n) == -2)
            {//правое поддерево дриннее на 2
                if(balanceFactor(n->right) == 1)
                {//правое поддерево длиннее слева
                    rotateRight(n->right);
                }
                rotateLeft(n);
            }
            node = parent(n);// в результате вращения узел из node мог замениться на другой. Он содержится в n
        }
    }


    void doInsert(Node *newNode, Node *parent) override
    {
        newNode->internalPtr = new info{parent, 1};//у листьев высота 1
        rebalance(parent);
    }

    void doRemove(Node *&nodeToRemove) override
    {
        Node* rebalanceStart = parent(nodeToRemove);
        delete nodeToRemove->internalPtr;
        BinarySearchTree::doRemove(nodeToRemove);
        rebalance(rebalanceStart);
    }
};

#endif // AVLTREE_H
