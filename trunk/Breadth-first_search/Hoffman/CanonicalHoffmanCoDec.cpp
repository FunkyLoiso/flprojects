#include "CanonicalHoffmanCoDec.h"
#include <algorithm>
#include <numeric>

template<size_t N>
void incrementBitSet(std::bitset<N>& set)
{
    size_t i = 0;
    for(; set.at(i) == true; ++i) set.reset(i);
    set.set(i);
}

CanonicalHoffmanCoDec::CanonicalHoffmanCoDec(const HoffmanTree& tree)
{
    auto getCodeLength = [](HoffmanTree::Node* n)
    {
        uint8_t length = 0;
        for(; n->parent != nullptr; ++length, n = n->parent);
        return length;
    };

    // 1. добавим все значения и длины соответствующих им кодов в m_codes
    for(auto leaf : tree.leaves)
    {
        codeInfo info;
        info.val = leaf.first;
        info.code.size = getCodeLength(leaf.second);
        m_codes.push_back(info);
    }

    if(m_codes.size() < 1) return;

    std::sort(m_codes.begin(), m_codes.end());//отсортируем коды по возрастанию длины кода, а внутри по возрастанию значения

    // 2. Рассчитаем канонические коды Хоффмана.
    HoffmanCode lastCode = m_codes.at(0).code;
    m_shifts.resize(m_codes.back().code.size+2, 0);//сюда сначала запишем число кодов каждой длины (в позицию длина+1), потом получим смещения, рассчитав и записав частичные суммы
    ++m_shifts.at(lastCode.size+1);
    m_valueToCodes[m_codes.at(0).val] = lastCode;
    for(auto i = m_codes.begin()+1; i != m_codes.end(); ++i)
    {
        HoffmanCode::TData newData = lastCode.data;
        incrementBitSet(newData);//новый код больше предыдущего на единицу
        if(i->code.size > lastCode.size) newData <<= 1;//если у нового значения код длиннее, то добавляем к увеличенному коду ноль справа
        i->code.data = newData;//запишем новый код в m_codes
        m_valueToCodes[i->val] = i->code;//и в m_valueToCodes
        lastCode = i->code;
        ++m_shifts.at(i->code.size+1);//увечиливаем счётчик кодов с такой длиной+1 (чтобы потом сумма в позиции code.size была равна смещению кодов такой длины)
    }

    //запишем в вектор смещений частичные суммы
    std::partial_sum(m_shifts.begin(), m_shifts.end(), m_shifts.begin());
}

CanonicalHoffmanCoDec::TSerialization CanonicalHoffmanCoDec::serialize() const
{
    return TSerialization();
}

CanonicalHoffmanCoDec CanonicalHoffmanCoDec::deserialize(const CanonicalHoffmanCoDec::TSerialization &data)
{
    return CanonicalHoffmanCoDec(HoffmanTree());
}

HoffmanCode CanonicalHoffmanCoDec::encode(uint8_t value) const
{
    auto i = m_valueToCodes.find(value);
    if(i != m_valueToCodes.end()) return i->second;
    else return HoffmanCode();
}

bool CanonicalHoffmanCoDec::decode(const HoffmanCode& code, uint8_t& outValue) const
{
    if(code.size >= m_shifts.size()) return false;// таких длинных кодов нет

    auto begin = m_codes.begin() + m_shifts.at(code.size);
    decltype(begin) end;
    if(code.size == m_shifts.size()-1) end = m_codes.end();
    else
    {
        int codeCount = m_shifts.at(code.size+1) - m_shifts.at(code.size);//число кодов длины code.size
        end = begin + codeCount;
    }

    auto hasCode = [&code] (const codeInfo& i) { return i.code.data == code.data; };
    auto i = std::find_if(begin, end, hasCode);//к сожалению std::bitset не предоставляет оператор сравнения, так что бинарный поиск не возможен (?)
    if( i != end)
    {
        outValue = i->val;
        return true;
    }
    return false;
}
