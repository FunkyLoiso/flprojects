#ifndef KNUTHMORRISPRATT_H
#define KNUTHMORRISPRATT_H

#include <string>
#include <vector>

namespace StringMatching
{
class KnuthMorrisPratt
{
public:
    KnuthMorrisPratt(std::string::iterator textBeg, std::string::iterator textEnd,
                     std::string::iterator patternBeg, std::string::iterator patternEnd);

    std::string::iterator nextMatch();

private:
    std::string::iterator m_textCur;
    const std::string::iterator m_patternBeg, m_patternEnd, m_textEnd;
    const int m_patternLength;
    int m_charsMatched; // текущее число совпавших с шаблоном символов для nextMatch

    //m_prefix[n] это количество начальных букв pattern, которые являются суффиксом первых n букв pattern
    //ex: для abcab m_prefix[3] = 0, m_prefix[4] = 1, m_prefix[5] = 2
    std::vector<int> m_prefix;

    void computePrefix();
};
}// namespace StrngMatching

#endif // KNUTHMORRISPRATT_H
