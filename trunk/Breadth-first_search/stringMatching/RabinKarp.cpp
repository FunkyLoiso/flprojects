#include "RabinKarp.h"
#include <cassert>

using namespace StringMatching;

template<typename TVal, typename TExp, typename TMod>
TVal modularExponent(TVal value, TExp exp, TMod modulus)
{
    if(exp == 0) return 1;
    value %= modulus;
    for(; exp > 1; --exp)
    {
        value = (value * value) % modulus;
    }
    return value;
}

RabinKarp::RabinKarp(std::string::iterator textBeg, std::string::iterator textEnd,
                     std::string::iterator patternBeg, std::string::iterator patternEnd)
    : m_textCur(textBeg), m_textEnd(textEnd), m_patternBeg(patternBeg), m_patternEnd(patternEnd)
    , m_patternVal(integerValue(patternBeg, patternEnd)) // P
    , m_patternLength(patternEnd - patternBeg)
    , m_h(modularExponent(radix, m_patternLength-1, q)) // P
{
    assert(textEnd - textBeg >= m_patternLength);
    m_textVal = integerValue(textBeg, textBeg + m_patternLength); // P
}

std::string::iterator RabinKarp::nextMatch()
{
    while(m_textCur != m_textEnd-m_patternLength+1)
    {
        bool done = false;
        if(m_textVal == m_patternVal)
        {
            done = std::equal(m_patternBeg, m_patternEnd, m_textCur);
        }

        if(m_textCur != m_textEnd-m_patternLength)
        {
            unsigned char curSignificant = *m_textCur;
            unsigned char newChar = *(m_textCur+m_patternLength);
            m_textVal = ( radix*(m_textVal - curSignificant * m_h) + newChar ) % q;
        }
        ++m_textCur;

        if(done) return m_textCur-1;
    }
    return m_textEnd;
}

unsigned int RabinKarp::integerValue(std::string::iterator beg, std::string::iterator end) const
{
    unsigned int result = 0;
    for(auto i = beg; i != end; ++i)
    {
        result = (radix * result + static_cast<unsigned char>(*i)) % q;
    }

    return result;
}
