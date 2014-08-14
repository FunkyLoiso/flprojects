#ifndef TESTHOFFMAN_H
#define TESTHOFFMAN_H

#include "HoffmanTree.h"
#include "CanonicalHoffmanCoDec.h"
#include "HoffmanArchiver.h"

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
//    std::ifstream file("c:\\test2.txt", std::ios_base::binary);
//    HoffmanTree t = HoffmanTree::build(file);
//    file.close();

//    CanonicalHoffmanCoDec codec(t);

//    for(int i = 0; i <= 255; ++i)
//    {
//        std::cout << char(i) << ": " << codec.encode(i) << std::endl;
//    }

//    for(int i = 0; i <= 0xffff; ++i)
//    {
//        for(int l = 0; l < 255; ++l)
//        {
//            uint8_t val;
//            HoffmanCode c;
//            c.data = i;
//            c.size = l;

//            if(codec.decode(c, val))
//            {
//                std::cout << c << ": " << char(val) << std::endl;
//                assert(c.data == codec.encode(val).data);
//            }
//        }
//    }

//    auto serialization = codec.serialize();
//    std::cout << "Codec serailized size: " << serialization.size() << " bytes" << std::endl;

//    //создадим кодек из сериализации и сравним с оригиналом
//    CanonicalHoffmanCoDec deser = CanonicalHoffmanCoDec::deserialize(serialization);

//    for(int i = 0; i <= 255; ++i)
//    {
//        assert(codec.encode(i).data == deser.encode(i).data);
//        assert(codec.encode(i).size == deser.encode(i).size);
//    }

//    for(int i = 0; i <= 0xffff; ++i)
//    {
//        for(int l = 0; l < 255; ++l)
//        {
//            uint8_t val;
//            HoffmanCode c;
//            c.data = i;
//            c.size = l;

//            if(codec.decode(c, val))
//            {
//                uint8_t deserVal;
//                assert(deser.decode(c, deserVal));
//                assert(val == deserVal);
//                assert(c.data == deser.encode(deserVal).data);
//            }
//        }
//    }


    //проверка архиватора
    std::ifstream srcFile("c:\\test8.txt", std::ios_base::binary);
    std::ofstream compressedFileO("c:\\test.compressed", std::ios_base::binary);

    HoffmanCompress(srcFile, compressedFileO);
    compressedFileO.close();
    srcFile.close();


    std::ifstream compressedFileI("c:\\test.compressed", std::ios_base::binary);
    std::ofstream decompressedFile("c:\\test_out.txt", std::ios_base::binary);
    HoffmanDecompress(compressedFileI, decompressedFile);
    compressedFileI.close();
    decompressedFile.close();
}

#endif // TESTHOFFMAN_H
