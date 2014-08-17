#ifndef HASHTABLEOPEN_H
#define HASHTABLEOPEN_H

template<typename Key, typename Value, typename Hasher = std::hash<Key>>
class HashTableOpen
{
public:
    HashTableOpen(const Hasher& hasher = Hasher()) : m_hasher(hasher) {}

    void insert(const Key& key, const Value& val)
    {
        float loadFactor = static_cast<float>(++m_elCount) / m_records.size();
        if(loadFactor > 0.8 || loadFactor < 0.1) rehash();

        for(size_t i = 0; i < m_records.size(); ++i)
        {
            record& r = recordSeq(key, i);
            if(r.state != record::Oqupied)
            {
                r.key = key;
                r.value = val;
                r.state = record::Oqupied;
                break;
            }
        }

    }

    Value* find(const Key& key)
    {
        for(size_t i = 0; i < m_records.size(); ++i)
        {
            record& r = recordSeq(key, i);
            if(r.state == record::Empty) return nullptr;
            if(r.state == record::Deleted) continue;
            if(r.key == key) return &r.value;
        }
        return nullptr;
    }

    bool erase(const Key& key)
    {
        for(size_t i = 0; i < m_records.size(); ++i)
        {
            record& r = recordSeq(key, i);
            if(r.state != record::Oqupied) return false;
            if(r.key == key)
            {
                r.state = record::Deleted;
                return true;
            }
        }
        return false;
    }

    void dump()
    {
        for(size_t i = 0; i < m_records.size(); ++i)
        {
            const record& r = m_records.at(i);
            std::cout << i << ": " << r.key << ";" << r.value << " state: " << r.state << std::endl;
        }
    }

private:
    struct record
    {
        Key key;
        Value value;
        enum {Empty, Deleted, Oqupied} state = Empty;
    };

    const Hasher& m_hasher;
    std::vector<record> m_records;
    size_t m_elCount = 0;

    record& recordSeq(const Key& key, size_t seqN)
    {
#define LINEAR
//#define QUADRATIC
#ifdef LINEAR
        size_t pos = ( m_hasher(key) % m_records.size() ) + seqN;
        if(pos >= m_records.size()) pos %= m_records.size();
        return m_records.at(pos);
#elif defined(QUADRATIC)
        size_t pos = ( m_hasher(key) % m_records.size() ) + seqN + seqN*seqN;
        if(pos >= m_records.size()) pos %= m_records.size();
        return m_records.at(pos);
#else //double hashing
        size_t pos = (m_hasher(key) + seqN*m_hasher(key+1)) % m_records.size();
        return m_records.at(pos);
#endif
    }

    void rehash()
    {
        decltype(m_records) oldRecords = std::move(m_records);
        m_records = decltype(m_records) (m_elCount*3);

        for(const record& oldR : oldRecords)
        {
            if(oldR.state != record::Oqupied) continue;

            for(size_t i = 0; i < m_records.size(); ++i)
            {
                record& newR = recordSeq(oldR.key, i);
                if(newR.state != record::Oqupied)
                {
                    newR = oldR;
                    break;
                }
            }
        }
    }
};

#endif // HASHTABLEOPEN_H
