#ifndef HASHTABLELINKEDLIST_H
#define HASHTABLELINKEDLIST_H

#include <vector>
#include <list>

#include <iostream>

template<typename Key, typename Value, typename Hasher = std::hash<Key>>
class HashTableLinkedList
{
public:
    HashTableLinkedList(const Hasher& hasher = Hasher()) : m_hasher(hasher) {}

    void insert(const Key& key, const Value& val)
    {
        float loadFactor = static_cast<float>(++m_elCount) / m_bins.size();
        if(loadFactor > 1.0 || loadFactor < 0.1) rehash();

        bin(key).push_front( std::make_pair(key, val) );
    }

    Value* find(const Key& key)
    {
        for(auto& p : bin(key))
        {
            if(p.first == key) return &p.second;
        }
        return nullptr;
    }

    bool erase(const Key& key)
    {
        auto& b = bin(key);
        for(auto i = b.begin(); i != b.end(); ++i)
        {
            if(i->first == key)
            {
                b.erase(i);
                return true;
            }
        }
        return false;
    }

    void dump()
    {
        int c = 0;
        for(auto& b : m_bins)
        {
            std::cout << "bin " << c++ << ": ";
            for(auto& p : b)
            {
                std::cout << p.first << ";" << p.second << ", ";
            }
            std::cout << std::endl;
        }
    }

private:
    using TBin = std::list<std::pair<Key, Value>>;
    std::vector<TBin> m_bins;
    const Hasher& m_hasher;
    size_t m_elCount = 0;

    inline TBin& bin(const Key& key)
    {
        return m_bins.at( m_hasher(key) % m_bins.size() );
    }

    void rehash()
    {
        decltype(m_bins) oldBins = std::move(m_bins);
        m_bins = decltype(m_bins) (m_elCount*3);
        for(auto& b : oldBins)
        {
            for(auto& p : b)
            {
                bin(p.first).push_front(p);
            }
        }
    }
};

#endif // HASHTABLELINKEDLIST_H
