#include "decoder.h"
#include "image.h"
#include <iostream>
#include <bitset>
#include <ios>
#include <cassert>

void RunTests ();

int main() {
    RunTests();

    //Decode("../tests/bad_quality.jpg");

    return 0;
}

void RunTests () {
    /* TEST SECTION */
    File file("../test.txt");

    assert(file.GetWord() == static_cast<uint16_t>(0x3132));
    assert(file.GetWord() == static_cast<uint16_t>(0x3132));
    assert(file.GetWord() == static_cast<uint16_t>(0x3531));
    assert(file.GetByte() == static_cast<uint8_t>(0x35));
    assert(file.GetByte() == static_cast<uint8_t>(0x32));
    uint8_t bitter = 0;
    for (size_t i = 0; i < 8; ++i) {
        bitter |= file.GetBit() << (7 - i);
    }
    assert(bitter == static_cast<uint8_t>(0x35));
    assert(file.GetWord() == static_cast<uint16_t>(0x3135));
    assert(file.GetByte() == static_cast<uint8_t>(0x32));
    file.SkipBits(15);
    /*
    for (size_t i = 0; i < 15; ++i) {
        file.GetBit();
    }*/
    assert(file.GetBit() == static_cast<uint8_t>(0));
    /*
    std::cout << std::bitset<8>(file.GetByte()) << std::endl;
    std::cout << std::bitset<8>(static_cast<uint8_t>(0x31)) << std::endl;
    std::cout << std::bitset<8>(file.GetByte()) << std::endl;
    std::cout << std::bitset<8>(static_cast<uint8_t>(0x34)) << std::endl;
    */
    assert(file.GetWord() == static_cast<uint16_t>(0x3134));
    assert(file.GetHalfByte() == static_cast<uint8_t>(0x3));
    assert(file.GetHalfByte() == static_cast<uint8_t>(0x1));
    assert(file.GetByte() == static_cast<uint8_t>(0x32));

    /*
    size_t size = 8;
    size_t counter = 0;
    auto DC_quantification_table = std::vector<std::vector<int>>(size, std::vector<int>(size));
    for (size_t j = 0; j < size; ++j) {
        for (size_t i = 0; i <= j; ++i) {
            if (j % 2) {
                DC_quantification_table[i][j - i] = counter++;
            } else {
                DC_quantification_table[j-i][i] = counter++;
            }
        }
    }

    bool is_odd = size % 2;

    for (size_t j = 1; j < size; ++j) {
        for (size_t i = size - 1; i >= j; --i) {
            if ((j + is_odd) % 2) {
                DC_quantification_table[i][j + size - 1 - i] = counter++;
            } else {
                DC_quantification_table[j + size - 1 - i][i] = counter++;
            }
        }
    }

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            std::cout << DC_quantification_table[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    */

    Decoder::RunTests();

    auto decode = Decoder(File("../bad_test.jpg"));
    decode.AssertNextByte(0xFF);
    decode.AssertNextByte(0xD8);
    decode.AssertNextWord(0xFFE0);
    /* */
}
