#ifndef TESTHOFFMAN_H
#define TESTHOFFMAN_H

#include "HoffmanTree.h"
#include "CanonicalHoffmanCoDec.h"

#include <fstream>
#include <iostream>
#include <cassert>

std::ostream& operator<<(std::ostream& stream, const HoffmanCode& c)
{
    for(int i = c.size-1; i >= 0; --i)
    {
        stream << c.data.at(i) ? 1 : 0;
        if(i % 8 == 0) stream << ' ';
    }
    return stream;
}

void testHoffman()
{
    std::ifstream file("c:\\test.txt");
    HoffmanTree t = HoffmanTree::build(file);
    file.close();

    CanonicalHoffmanCoDec codec(t);

    for(int i = 0; i <= 255; ++i)
    {
//        std::cout << char(i) << ": " << t.code(i) << std::endl;
        std::cout << char(i) << ": " << codec.encode(i) << std::endl;
    }

    for(int i = 0; i <= 0xffff; ++i)
    {
        for(int l = 0; l < 255; ++l)
        {
            uint8_t val;
            HoffmanCode c;
            c.data = i;
            c.size = l;

            if(codec.decode(c, val))
            {
                std::cout << c << ": " << char(val) << std::endl;
                assert(c.data == codec.encode(val).data);
            }
        }
    }
}

#endif // TESTHOFFMAN_H
