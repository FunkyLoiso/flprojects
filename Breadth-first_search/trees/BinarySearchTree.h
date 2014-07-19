#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <sstream>
#include <stack>
#include <unordered_set>
#include <assert.h>

template<typename Key, typename Value = Key>
class BinarySearchTree
{
public:
    BinarySearchTree() {}
    BinarySearchTree(const BinarySearchTree& other)
    {
        auto inserter = [this](const Key& key, const Value& val)
        {
            insert(key, val);
            return false;
        };
        other.traversePreorder(inserter);
    }

    BinarySearchTree& operator=(BinarySearchTree other)
    {// other передаётся по значению, создаваясь конструктором копирования, так что мы можем просто взять указатель m_head.
     // Вызов swap вместо простого копирования необходим потому, что в деструкоторе other будет освобождена память,
     // на которую указывает его поле m_head.
        std::swap(m_head, other.m_head);
        return *this;
    }

    ~BinarySearchTree()
    {
        while(m_head != nullptr) remove(m_head->key);
    }

    virtual void insert(const Key& key, const Value& value)
    {
        Node** cur = &m_head;
        for(;;)
        {
            Node* node = *cur;
            if(node == nullptr)
            {
                *cur = new Node{key, value, nullptr, nullptr};
                return;
            }
            else if(key < node->key)
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

    void insert(const Key& key)
    {
        static_assert(std::is_same<Key, Value>::value,
                      "Key-only insertion is available if Key and Value types are equal");
        insert(key, key);
    }

    void insert(const std::vector<Key> keys)
    {
        for(const Key& key : keys) insert(key);
    }

    Value* find(const Key& key) const
    {
        Node** ptr = findPtr(key);
        if(*ptr == nullptr) return nullptr;
        else return &(*ptr)->val;
    }

    virtual bool remove(const Key& key)
    {
        Node** ptr = findPtr(key);//адрес поля родителя, указывающей на удаляемый узел
        if(*ptr == nullptr) return false;// не найдено

        for(;;)
        {
            Node* node = *ptr;

            if(node->left != nullptr && node->right != nullptr)
            {// оба ребёнка есть
                //найдём самый левый узел в правом поддереве
                Node** succ = inOrderSuccessor(node);
                //перенесём его ключ и значение в удаляемый узел
                node->key = (*succ)->key;
                node->val = (*succ)->val;
                //удалим его вместо удаляемого узла
                ptr = succ;
                continue;
            }
            else
            {
                if(node->left == nullptr && node->left == nullptr)
                {// детей нет, это лист
                    *ptr = nullptr;
                }
                else
                {// есть один ребёнок
                    Node* child = node->left != nullptr ? node->left : node->right;
                    *ptr = child;
                }
                delete node;
                return true;
            }
        }


    }

    using fn = std::function<bool(const Key&, const Value&)>;
    void traversePreorder(fn func) const
    {
        std::stack<Node*> stack;
        stack.push(m_head);
        while (!stack.empty())
        {
            Node* node = stack.top();
            stack.pop();
            if(node != nullptr)
            {
                bool done = func(node->key, node->val);
                if(done) return;
                stack.push(node->right);
                stack.push(node->left);
            }
        }
    }

    void traverseInorder(fn func) const
    {
        std::unordered_set<Node*> visited;
        std::stack<Node*> stack;
        stack.push(m_head);
        while (!stack.empty())
        {
            Node* node = stack.top();
            if(node != nullptr)
            {
                if(visited.count(node->left) == 0)
                {// первое посещение - добавим левую ветвь
                    stack.push(node->left);
                    continue;
                }
                else
                {// второе посещение, обработаем, добавим правую ветвь, уберём из стека
                    bool done = func(node->key, node->val);
                    if(done) return;
                    stack.pop();
                    visited.insert(node);
                    stack.push(node->right);
                }
            }
            else
            {
                stack.pop();
                visited.insert(node);
            }
        }
    }

    void traversePostorder(fn func) const
    {
        std::unordered_set<Node*> visited;
        std::stack<Node*> stack;
        stack.push(m_head);
        while (!stack.empty())
        {
            Node* node = stack.top();
            if(node != nullptr)
            {
                if(visited.count(node->left) == 0)
                {// первое посещение - добавим левую ветвь
                    stack.push(node->left);
                    continue;
                }
                else if(visited.count(node->right) == 0)
                {// второе посещение, добавим правую ветвь
                    stack.push(node->right);
                    continue;
                }
                else
                {// третье посещение, обработаем, удалим из стека
                    bool done = func(node->key, node->val);
                    if(done) return;
                    stack.pop();
                    visited.insert(node);
                }
            }
            else
            {
                stack.pop();
                visited.insert(node);
            }
        }
    }

    std::string toString() const
    {
        std::ostringstream str;
        printNode(str, m_head, 0);
        return str.str();
    }

   private:
    struct Node
    {
        Key key; Value val; Node* left; Node* right;
        std::string toString()
        {
            if(std::is_same<Key, Value>::value) return std::to_string(key);
            else return std::string("(") + std::to_string(key) + ", " + std::to_string(val) + ")";
        }
    }* m_head = nullptr;

    void printNode(std::ostringstream& str, Node* node, int spaces) const
    {
        if(node == nullptr) return;

        str << std::string(spaces, ' ') << node->toString() << std::endl;
        printNode(str, node->left, spaces+1);
        printNode(str, node->right, spaces+1);
    }

    // Возвращает адрес родительского поля, указывающего на искомый узел
    Node** findPtr(const Key& key)
    {
        Node** cur = &m_head;
        for(;;)
        {
            if(*cur == nullptr) return cur;
            else if(key == (*cur)->key) return cur;
            else if(key < (*cur)->key) cur = &((*cur)->left);
            else cur = &((*cur)->right);
        }
    }

    // Возвращает следующий по ключу узел, то есть самый глубокий левый узел в правом поддереве
    Node** inOrderSuccessor(Node*& node)
    {
        assert(node != nullptr);
        assert(node->right != nullptr);
        Node** result = &(node->right);
        while((*result)->left != nullptr) result = &((*result)->left);
        return result;
    }
   // Возвращает предыдущий по ключу узел, то есть самый глубокий правый узел в левом поддереве
    Node** inOrderPredecessor(Node*& node)
    {
        assert(node != nullptr);
        assert(node->left != nullptr);
        Node** result = &(node->left);
        while((*result)->right != nullptr) result = &((*result)->right);
        return result;
    }
};

#endif // BINARYSEARCHTREE_H
