#ifndef HOFFMANARCHIVER_H
#define HOFFMANARCHIVER_H

#include <istream>

#include "CanonicalHoffmanCoDec.h"

class HoffmanCodeWriter
{
public:
    HoffmanCodeWriter(std::ostream& dst) : m_dst(dst) {}
    uint8_t write(const HoffmanCode& code)
    {
        uint8_t written = 0;
        for(int codePos = code.size-1; codePos >= 0; --codePos)
        {
            m_buf.set(m_pos--, code.data.test(codePos));
            if(m_pos < 0)
            {//буфер заполнился
                m_dst.put(char(m_buf.to_ulong()));
                written += 8;
                m_pos = 7;
            }
        }
        return written;
    }

    uint8_t finalyze()
    {
        if(m_pos != 7)
        {
            m_dst.put(char(m_buf.to_ulong()));
            return m_pos + 1;
        }
        else return 0;
    }



private:
    std::ostream& m_dst;
    std::bitset<8> m_buf;
    int m_pos = 7;

};

void HoffmanCompress(std::istream& src, std::ostream& dst)
{
    auto tree = HoffmanTree::build(src);
    CanonicalHoffmanCoDec codec(tree);

    src.clear();
    src.seekg(0);

    CanonicalHoffmanCoDec::TSerialization ser = codec.serialize();
    uint16_t serSize = static_cast<uint16_t> (ser.size());
    dst.write(reinterpret_cast<char *>(&serSize), sizeof(serSize));

    dst.write(reinterpret_cast<char *>(ser.data()), ser.size());

    auto extraBitsPos = dst.tellp();//место, в которое нужно будет вписать количество лишних битов
    dst.seekp(1, std::ios_base::cur);//сдвинемся на 1 вперёд

    HoffmanCodeWriter writer(dst);
//    size_t dataRead = 0;//debug
    while(src.good())
    {
        uint8_t data = src.get();
//        dataRead += 8;
        if(src.good())
        {
            writer.write(codec.encode(data));
        }
    }
    uint8_t extraBitsCount = writer.finalyze();
    dst.seekp(extraBitsPos);
    dst.put(extraBitsCount);
}

void HoffmanDecompress(std::istream& src, std::ostream& dst)
{
    uint16_t serSize;
    src.read(reinterpret_cast<char*>(&serSize), sizeof(serSize));

    CanonicalHoffmanCoDec::TSerialization ser(serSize);
    src.read(reinterpret_cast<char*>(ser.data()), ser.size());

    CanonicalHoffmanCoDec codec = CanonicalHoffmanCoDec::deserialize(ser);

    uint8_t extraBitsCount = src.get();

    //выясним, до куда нужно читать, чтобы не прочитать лишние биты
    auto curPos = src.tellg();
    size_t bitsLeft = src.seekg(0, std::ios_base::end).tellg() - curPos;// нужен ли -1?
    bitsLeft = bitsLeft * 8 - extraBitsCount;
    src.seekg(curPos);

    HoffmanCode code;
    uint8_t value;
//    size_t dataRead = 0;//debug
    while (src.good() && bitsLeft > 0)
    {
        std::bitset<8> data = src.get();
//        dataRead += 8;
        for(int pos = 7; pos >= 0; --pos)
        {
            code.data <<= 1;
            code.data.set(0, data.test(pos));
            ++code.size;

            if(codec.decode(code, value))
            {
                dst.put(value);
                code.data = 0;
                code.size = 0;
            }

            if(--bitsLeft == 0) break;
        }
    }
//    _CrtDbgBreak();
}

#endif // HOFFMANARCHIVER_H
