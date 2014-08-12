#ifndef TESTHOFFMAN_H
#define TESTHOFFMAN_H

#include "HoffmanTree.h"

#include <fstream>
#include <iostream>

std::ostream& operator<<(std::ostream& stream, const HoffmanTree::Code& c)
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

    for(int i = 0; i <= 255; ++i)
    {
        std::cout << char(i) << ": " << t.code(i) << std::endl;
    }
}

#endif // TESTHOFFMAN_H
