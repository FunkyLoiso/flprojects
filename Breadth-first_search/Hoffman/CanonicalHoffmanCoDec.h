#ifndef CANONICALHOFFMANCODEC_H
#define CANONICALHOFFMANCODEC_H

#include "HoffmanTree.h"

#include <vector>

class CanonicalHoffmanCoDec
{
public:
    CanonicalHoffmanCoDec(const HoffmanTree& tree);

    using TSerialization = std::vector<uint8_t>;
    TSerialization serialize() const;
    static CanonicalHoffmanCoDec deserialize(const TSerialization& data);

    HoffmanCode encode(uint8_t value) const;
    bool decode(const HoffmanCode& code, uint8_t& outValue) const;

private:
    std::unordered_map<uint8_t, HoffmanCode> m_valueToCodes;

    struct codeInfo
    {
        uint8_t val;
        HoffmanCode code;
        bool operator<(codeInfo other)
        {//сравнение по длине кода, внутри по значению
            return ( code.size != other.code.size ? code.size < other.code.size : val < other.val );
        }
    };

    std::vector<codeInfo> m_codes;
    std::vector<int> m_shifts;//смещения. с m_codes.begin() + m_shifts[i] начинаются коды длины i, i == 0, 1, 2...
};

#endif // CANONICALHOFFMANCODEC_H
