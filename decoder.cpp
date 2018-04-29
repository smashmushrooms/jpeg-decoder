#include "decoder.h"

Image Decode(const std::string& filename) {
    auto file = File(filename);
    auto decoder = Decoder(std::move(file));

    decoder.SOI();
    decoder.APP0();
    decoder.COM();
    decoder.DQT();
    decoder.DQT();
    decoder.SOF0();
    decoder.DHT();
/*
    SOI(); // xFFD8 Start of Image
    APP0(); // xFFE0 ?
    COM(); // xFFFE Get comment
    DQT(); // xFFDB Define a Quantization Table. x2

    SOF0(); // xFFC0 Baseline DCT
    DHT(); // xFFC4 Define Huffman Table x4

    SOS(); // xFFDA Start of Scan

    EOI(); // End of Image xFFD9
    */
    return Image();
}