#ifndef RABINKARP_H
#define RABINKARP_H

#include <string>

namespace StringMatching
{
class RabinKarp
{
public:
    RabinKarp(std::string::iterator textBeg, std::string::iterator textEnd,
              std::string::iterator patternBeg, std::string::iterator patternEnd);

    //возвращает итератор на очередное начало pattern в text. Возврашает textEnd, если больше совпадений нет.
    std::string::iterator nextMatch();

private:
    static const unsigned int radix = 256;//основание системы символов
    static const unsigned int q = 16777127;//большое простое число, такое что radix * q вмещается в регистр

    std::string::iterator m_textCur;
    const std::string::iterator m_patternBeg, m_patternEnd, m_textEnd;

    const unsigned int m_patternVal;//численное представление шаблона
    unsigned int m_textVal = 0;//численное представление текущей порции текста
    const int m_patternLength;
    int m_h;// значение старшего разряда по модулю q

    unsigned int integerValue(std::string::iterator beg, std::string::iterator end) const;
};
}//namespace StringMatching

#endif // RABINKARP_H
