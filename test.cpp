#include "decoder.h"

void Decoder::RunTests() {
    auto decoder = Decoder(File("../tests/bad_quality.jpg"));

    TEST("SOI correctness", [&]() -> void {
        decoder.SOI();
    });

    TEST("APP0 correctness", [&]() -> void {
        decoder.APP0();
    });

    TEST("COM correctness", [&]() -> void {
        decoder.COM();
        ASSERT(decoder.image_.GetComment() == "so quality");
    });

    TEST("DQT DC table", [&]() -> void {
        decoder.DQT();
        std::vector<std::vector<int>> DC_TABLE = {
                {80,  55,  50,  80, 120, 200, 255, 255},
                {60,  60,  70,  95, 130, 255, 255, 255},
                {70,  65,  80, 120, 200, 255, 255, 255},
                {70,  85, 110, 145, 255, 255, 255, 255},
                {90, 110, 185, 255, 255, 255, 255, 255},
                {120, 175, 255, 255, 255, 255, 255, 255},
                {245, 255, 255, 255, 255, 255, 255, 255},
                {255, 255, 255, 255, 255, 255, 255, 255}
        };
        ASSERT(decoder.quantification_tables_[DC] == DC_TABLE);
    });

    TEST("DQT AC table", [&]() -> void {
        decoder.DQT();
        std::vector<std::vector<int>> AC_TABLE = {
                {85,  90, 120, 235, 255, 255, 255, 255},
                {90, 105, 130, 255, 255, 255, 255, 255},
                {120, 130, 255 ,255, 255, 255, 255, 255},
                {235, 255, 255, 255, 255, 255, 255, 255},
                {255, 255, 255, 255, 255, 255, 255, 255},
                {255, 255, 255, 255, 255, 255, 255, 255},
                {255, 255, 255, 255, 255, 255, 255, 255},
                {255, 255, 255, 255, 255, 255, 255, 255}
        };
        ASSERT(decoder.quantification_tables_[AC] == AC_TABLE);
    });

    TEST("SOF0 corectness", [&]() -> void {
        decoder.SOF0();
        ASSERT(decoder.image_.Width() == 990);
        ASSERT(decoder.image_.Height() == 560);
        ASSERT(decoder.components_[0] == Component(1, 1, 2, 0));
        ASSERT(decoder.components_[1] == Component(2, 1, 1, 1));
        ASSERT(decoder.components_[2] == Component(3, 1, 1, 1));
    });

    TEST("DHT DC0", [&]() -> void {
        decoder.DHT();
        std::vector<std::list<uint8_t>> DC0_table = {
                {0},
                {1},
                {2},
                {3},
                {4},
                {5},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
        };
        ASSERT(decoder.tables_[DC][0] == DC0_table);
    });

    TEST("DHT AC0", [&]() -> void {
        decoder.DHT();
        std::vector<std::list<uint8_t>> AC0_table = {
                {0x00},
                {0x01},
                {},
                {0x02, 0x11},
                {0x31, 0x41},
                {0x12, 0x21},
                {0x03, 0x51},
                {0x22, 0x61},
                {0x13, 0x71},
                {0x32, 0x52},
                {0x42, 0x91},
                {0x04, 0x62, 0x81},
                {},
                {0x82, 0xA1, 0xB1},
                {0xE1},
                {},
        };
        ASSERT(decoder.tables_[AC][0] == AC0_table);
    });

    decoder.DHT();
    decoder.DHT();

    TEST("SOS correctness", [&]() -> void {
    });
    decoder.SOS();
    decoder.EOI();

    __print__("");
    __print__("Here comes another tests!");
    __print__("");

    /*******************************************/

    auto google_img = Decoder(File("../60669610479084c8ecd776f5a2acd10c.jpg"));
    TEST("Decoding correctness", [&]() -> void {
        google_img.SOI();
        //google_img.APP0();
        google_img.COM();
        ASSERT(google_img.image_.GetComment() == ":)");
        google_img.DQT();
        google_img.DQT();
        google_img.SOF0();
        google_img.DHT();
        google_img.DHT();
        google_img.DHT();
        google_img.DHT();
        //__print_table__(google_img.tables_[DC][0]);
    });
    google_img.SOS();

    auto google_img1 = Decoder(File("../60669610479084c8ecd776f5a2acd10c.jpg"));
    TEST("SOS correctness", [&]() -> void {
        google_img1.SOI();
        //google_img.APP0();
        google_img1.COM();
        ASSERT(google_img1.image_.GetComment() == ":)");
        google_img1.DQT();
        google_img1.DQT();
        google_img1.SOF0();
        google_img1.DHT();
        google_img1.DHT();
        google_img1.DHT();
        google_img1.DHT();

        google_img1.AssertNextWord(0xFFDA, "Expected Start of Scan");
        google_img1.GetCurrStructureLen();

        google_img1.AssertNextByte(0x03, "Only 3 components can be supported");

        for (size_t i = 0; i < 3; ++i) {
            size_t component_id = google_img1.file_.GetByte();
            if (component_id == 0 || component_id > 3) {
                throw std::runtime_error("Wrong component id");
            }
            --component_id;
            google_img1.components_[component_id].trees.resize(2);
            auto DC_table_id = google_img1.file_.GetHalfByte();
            google_img1.AssertBit(DC_table_id);
            google_img1.components_[component_id].trees[DC] = google_img1.trees_[DC][DC_table_id];
            auto AC_table_id = google_img1.file_.GetHalfByte();
            google_img1.AssertBit(DC_table_id);
            google_img1.components_[component_id].trees[AC] = google_img1.trees_[AC][AC_table_id];
        }

        for (size_t i = 0; i < 3; ++i) {
            if (google_img1.components_[i].trees.empty()) {
                throw std::runtime_error("Same components are detected");
            }
            google_img1.file_.GetByte();
        }


        /* Odd number of components may emerge while thinning */

        auto number_of_Y_components_h = google_img1.GetNumberOfComponentsByOneDimension(
                google_img1.image_.Width(), google_img1.hth_max);
        auto number_of_Y_components_v = google_img1.GetNumberOfComponentsByOneDimension(
                google_img1.image_.Height(), google_img1.vth_max);

        std::vector<std::vector<int>> Y;
        for (size_t i = 0; i < number_of_Y_components_h; ++i) {
            for (size_t j = 0; j < number_of_Y_components_v; ++j) {
                for (size_t i_1 = 0; i_1 < google_img1.components_[0].vth; ++i_1) {
                    for (size_t j_1 = 0; j_1 < google_img1.components_[0].hth; ++j_1) {
                        Y.clear();
                        Y.resize(8, std::vector<int>(8));
                        google_img1.ReadDC(Y, 0);
                        google_img1.ReadAC(Y, 0);
                        google_img1.Dequant(Y, google_img1.components_[0].qt_id);
                    }
                }
            }
        }

        ASSERT(Y == std::vector<std::vector<int>>{
                {-160, 220, 200, 160, 0, 0, 0, 0},
                {-120, 0, -140, 0, 0, 0, 0, 0},
                {-140, -130, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0}
        });
    });

}
