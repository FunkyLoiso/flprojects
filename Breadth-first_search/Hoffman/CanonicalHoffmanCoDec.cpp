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

CanonicalHoffmanCoDec::CanonicalHoffmanCoDec(std::shared_ptr<HoffmanTree> tree)
{
    auto getCodeLength = [](HoffmanTree::Node* n)
    {
        uint8_t length = 0;
        for(; n->parent != nullptr; ++length, n = n->parent);
        return length;
    };

    // 1. добавим все значения и длины соответствующих им кодов в m_codes
    for(auto leaf : tree->leaves)
    {
        codeInfo info;
        info.val = leaf.first;
        info.code.size = getCodeLength(leaf.second);
        m_codes.push_back(info);
    }

    if(m_codes.size() < 1) return;

    std::sort(m_codes.begin(), m_codes.end());//отсортируем коды по возрастанию длины кода, а внутри по возрастанию значения

    // 2. Рассчитаем канонические коды Хоффмана.
    calculateHoffmanCodes();
}

void CanonicalHoffmanCoDec::calculateHoffmanCodes()
{
    HoffmanCode lastCode = m_codes.at(0).code;
    m_shifts.resize(m_codes.back().code.size+2, 0);//сюда сначала запишем число кодов каждой длины (в позицию длина+1), потом получим смещения, рассчитав и записав частичные суммы
    ++m_shifts.at(lastCode.size+1);
    m_valueToCodes[m_codes.at(0).val] = lastCode;
    for(auto i = m_codes.begin()+1; i != m_codes.end(); ++i)
    {
        HoffmanCode::TData newData = lastCode.data;
        incrementBitSet(newData);//новый код больше предыдущего на единицу
        newData <<= (i->code.size - lastCode.size);//если у нового значения код длиннее, то дополним код нулями справа
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
    //необходимо сформировать два массива: количества кодов каждой длинны и значения в порядке возрастания длины кода.
    //формат сериализации таков: |число элементов массива количеств|количества начиная с 1 ...|значения...|
    TSerialization result;
    uint8_t sizesCount = m_codes.back().code.size;
    result.push_back(sizesCount);

    //количества
    for(int i = 0; i < sizesCount; ++i)
    {
        uint8_t count = static_cast<uint8_t>(m_shifts.at(i+2) - m_shifts.at(i+1));// здесь может получиться 256, то есть 0, если все коды одинаковой длины
        result.push_back(count);
    }

    //величины
    for(const codeInfo& c : m_codes)
    {
        result.push_back(c.val);
    }

    return result;
}

CanonicalHoffmanCoDec CanonicalHoffmanCoDec::deserialize(const CanonicalHoffmanCoDec::TSerialization &data)
{
    CanonicalHoffmanCoDec codec;

    uint8_t sizesCount = data.at(0);
    std::vector<int> sizes(data.begin()+1, data.begin()+1+sizesCount); //int по описанной ниже причине
    //Возможна особая ситуация, когда все 256 величин распределены почти равномерно и потому все 256 кодов имеют одинаковую длину.
    //В таком случае sizesCount == 8, но все размеры равны 0, так как uint8_t(256) == 0. Обработаем эту ситуацию.
    if(sizesCount == 8 && sizes.at(7) == 0) sizes.at(7) = 256;
    auto valuesI = data.begin() + 1 + sizesCount;

    uint8_t curSize = 1;
    for(; valuesI != data.end(); ++valuesI)
    {
        while(sizes.at(curSize-1) == 0) ++curSize;

        codeInfo info;
        info.val = *valuesI;
        info.code.size = curSize;
        codec.m_codes.push_back(info);
        --sizes[curSize-1];
    }

    //m_codes уже отсортирован, осталось рассчитать канонические коды Хоффмана
    codec.calculateHoffmanCodes();

    return codec;
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
