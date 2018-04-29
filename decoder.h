#pragma once

#include "image.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cassert>
#include <bitset>
#include <unordered_map>
#include <list>

constexpr size_t BLOCK_SIZE = 8;

template <typename T = char[1]>
void __print__(const T &prontable = "") {
    std::cout << prontable << std::endl;
}

template <typename T>
void __print_bit__(const T &prontable) {
    std::cout << std::bitset<sizeof(T) * 8>(prontable) << std::endl;
}

template <typename T>
void __print_vector__(const T &vector) {
    for (auto&& element : vector) {
        std::cout << element << ' ';
    }
    std::cout << std::endl;
}

template <typename T>
void __print_table__(const T &table) {
    for (auto&& row : table) {
        for (auto&& element : row) {
            std::cout << element - 0 << ' ';
        }
        std::cout << std::endl;
    }
}

template <class T>
void ASSERT(const T& val, const std::string& error_message = "") {
    if (!val) {
        throw std::runtime_error(error_message);
    }
}

bool ASSERT_EQUALITY(bool val) {
    if (!val) {
        throw std::runtime_error("Expected equality!\n");
    }
}

template <class T>
void TEST(const std::string& test_name, T func) {
    try {
        func();
        std::cout << "Passed test: " << test_name << std::endl;
    } catch (...) {
        std::cerr << "Failed test: " << test_name << std::endl;
    }
}

Image Decode(const std::string& filename);

class File {
public:
    File() = delete;
    File(File&) = delete;
    File(File&& file)
            : file_stream_(std::move(file.file_stream_))
            , curr_byte_(file.curr_byte_)
            , bit_counter_(file.bit_counter_) {}

    File(const std::string& filename)
            : file_stream_(filename)
            , curr_byte_(0)
            , bit_counter_(0) {
        if (!file_stream_.is_open()) {
            throw std::runtime_error("Can not open file!");
        }
    }

    uint8_t GetByte() {
        if (file_stream_.peek() == EOF) {
            throw std::runtime_error("Unexpected EOF!");
        }
        if (bit_counter_) {
            throw std::runtime_error("Now the bit is reading!");
        }

        char byte;
        file_stream_.read(&byte, 1);
        curr_byte_ = byte;

        return curr_byte_;
    }

    uint8_t GetBit() {
        if (!bit_counter_) {
            GetByte();
        }

        uint8_t ans = (curr_byte_ & (0b10000000 >> bit_counter_++)) != 0;

        if (!(bit_counter_ % 8)) {
            bit_counter_ = 0;
        }

        return ans;
    }

    uint16_t GetWord() {
        GetByte();
        uint16_t word = curr_byte_ << 8;
        GetByte();
        word |= curr_byte_;

        return word;
    }

    std::string ReadString(size_t size) {
        char c_str[size + 1];
        file_stream_.read(c_str, size);
        auto str = std::string(c_str, c_str + size);
        if (str.size() != size) {
            throw std::runtime_error("Unexpected EOF!");
        }

        return str;
    }

    uint8_t GetHalfByte() {
        uint8_t byte = 0;
        for (size_t i = 0; i < 4; ++i) {
            byte |= GetBit() << (3 - i);
        }
        return byte;
    }

    void SkipBits(size_t n) {
        if (!bit_counter_) {
            GetByte();
        }
        if (n / 8) {
            for (size_t i = 0; i < n / 8; ++i) {
                if (file_stream_.peek() == EOF) {
                    throw std::runtime_error("Unexpected EOF!");
                }
                char byte;
                file_stream_.read(&byte, 1);
                curr_byte_ = byte;
            }
        }
        bit_counter_ += n % 8;
    }

    void Flush() {
        bit_counter_ = 0;
    }

private:
    std::ifstream file_stream_;

    uint8_t curr_byte_;
    size_t bit_counter_;
};

class HuffmanTree {
    struct Node;

public:
    HuffmanTree() = default;

    HuffmanTree(const std::vector<std::list<uint8_t>>& table, bool verbosity = false)
            : table_(table), root_(std::make_shared<Node>()), curr_(root_), verbosity_(verbosity) {
        Fill(root_);
    }

    void Fill(std::shared_ptr<Node> node, size_t depth = 0) {
        if (depth == 17) {
            throw std::runtime_error("Too large table");
        }
        ++counter;
        if (depth == 0 || table_[depth - 1].empty()) {
            if (!table_.number_of_codes) {
                if (verbosity_) {
                    std::cout << "\"NULL\";\n";
                }
                return;
            }
            node->left = std::make_shared<Node>();
            node->right = std::make_shared<Node>();
            auto temp_counter = counter;
            if (verbosity_) {
                std::cout << "\"NULL" << temp_counter << "\" -> ";
            }
            Fill(node->left, depth + 1);
            if (verbosity_) {
                std::cout << "\"NULL" << temp_counter << "\" -> ";
            }
            Fill(node->right, depth + 1);
            return;
        }

        node->value = table_[depth - 1].front();
        if (verbosity_) {
            std::cout << "\"" << node->value - 0 << "\";\n";
        }

        table_[depth - 1].pop_front();
        --table_.number_of_codes;
    }

    uint8_t DecodeNext(File* file) {
        curr_ = root_;
        while (curr_->left != nullptr || curr_->right != nullptr) {
            auto bit = file->GetBit();
            /*
            std::cout << (curr_->left == nullptr);
            std::cout << ' ';
            std::cout << (curr_->right == nullptr);
            std::cout << ' ';
            __print__(bit-0);*/
            if (bit) {
                ASSERT(curr_->right, "Right");
                curr_ = curr_->right;
            } else {
                ASSERT(curr_->left, "Left");
                curr_ = curr_->left;
            }
        }
        /*
        std::cout << (curr_->left == nullptr);
        std::cout << ' ';
        std::cout << (curr_->right == nullptr);
        std::cout << ' ';
        __print__();
         */
        auto res = curr_->value;
        return res;
    }

private:
    struct Node {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
        uint8_t value =  255;
    };

    struct HuffmanTable : public std::vector<std::list<uint8_t>> {
        HuffmanTable() = default;

        HuffmanTable(const std::vector<std::list<uint8_t>>& table)
                : std::vector<std::list<uint8_t>>(table), number_of_codes(0) {
            for (auto&& list : *this) {
                number_of_codes += list.size();
            }
        }

        size_t number_of_codes;
    };

    bool verbosity_;
    int counter = 0;

    std::shared_ptr<Node> root_;
    std::shared_ptr<Node> curr_;

    HuffmanTable table_;
};

class Decoder {
public:
    Decoder(File&& file)
            : file_(std::move(file))
            , tables_(2, std::vector<std::vector<std::list<uint8_t>>>(2))
            , trees_(2, std::vector<HuffmanTree>(2))
            , quantification_tables_(2) {}

    void SOI() {
        AssertNextWord(0xFFD8, "Expected SOI");
    }

    void APP0() {
        AssertNextWord(0xFFE0, "Expected APP0");
        GetCurrStructureLen();
        for (auto i = 0; i < curr_struct_len; ++i) {
            file_.GetByte();
        }
    }

    void COM() {
        AssertNextWord(0xFFFE, "Expected COM");
        GetCurrStructureLen();
        auto comment = file_.ReadString(curr_struct_len);
        image_.SetComment(comment);
    }

    void DQT() {
        AssertNextWord(0xFFDB, "Expected DQT");
        GetCurrStructureLen();

        bool is_one_byte_sized = !file_.GetHalfByte();
        AssertBit(is_one_byte_sized);
        auto id = file_.GetHalfByte();
        AssertBit(id);

        size_t size = std::lround(std::sqrt(curr_struct_len - 1));
        if (size * size != curr_struct_len - 1 || size != BLOCK_SIZE) {
            throw std::runtime_error("Incorrect size of QT");
        }

        if (is_one_byte_sized) {
            FillQTWith(quantification_tables_[id], file_, &File::GetByte, size);
        } else {
            FillQTWith(quantification_tables_[id], file_, &File::GetWord, size);
        }
    }

    void SOF0() {
        AssertNextWord(0xFFC0, "Expected SOF0");
        GetCurrStructureLen();

        precision_ = file_.GetByte();
        size_t height_ = file_.GetWord();
        size_t width_ = file_.GetWord();
        numer_of_components_ = file_.GetByte();

        image_.SetSize(width_, height_);

        for (size_t i = 0; i < numer_of_components_; ++i) {
            components_.emplace_back(file_.GetByte(), file_.GetHalfByte(),
                                     file_.GetHalfByte(), file_.GetByte());
            hth_max = std::max(hth_max, components_.back().hth);
            vth_max = std::max(vth_max, components_.back().vth);
        }

    }

    void DHT() {
        AssertNextWord(0xFFC4, "Expected DHT");
        GetCurrStructureLen();

        bool is_AC = file_.GetHalfByte();
        AssertBit(is_AC);
        TableType type = (is_AC) ? AC : DC;

        size_t table_id = file_.GetHalfByte();
        if (table_id > 1) {
            throw std::runtime_error("Bad table id");
        }

        std::vector<size_t> number_of_huffman_codes(16);
        for (size_t i = 0; i < 16; ++i) {
            number_of_huffman_codes[i] = file_.GetByte();
        }

        tables_[type][table_id].resize(16);

        for (size_t i = 0; i < 16; ++i) {
            for (size_t j = 0; j < number_of_huffman_codes[i]; ++j) {
                tables_[type][table_id][i].push_back(file_.GetByte());
            }
        }

        trees_[type][table_id] = HuffmanTree(tables_[type][table_id]);
    }

    void SOS() {
        AssertNextWord(0xFFDA, "Expected Start of Scan");
        GetCurrStructureLen();

        AssertNextByte(0x03, "Only 3 components can be supported");

        for (size_t i = 0; i < 3; ++i) {
            size_t component_id = file_.GetByte();
            if (component_id == 0 || component_id > 3) {
                throw std::runtime_error("Wrong component id");
            }
            --component_id;
            components_[component_id].trees.resize(2);
            auto DC_table_id = file_.GetHalfByte();
            AssertBit(DC_table_id);
            components_[component_id].trees[DC] = trees_[DC][DC_table_id];
            auto AC_table_id = file_.GetHalfByte();
            AssertBit(DC_table_id);
            components_[component_id].trees[AC] = trees_[AC][AC_table_id];
        }

        for (size_t i = 0; i < 3; ++i) {
            if (components_[i].trees.empty()) {
                throw std::runtime_error("Same components are detected");
            }
            file_.GetByte();
        }


        /* Odd number of components may emerge while thinning */

        auto number_of_Y_components_h = GetNumberOfComponentsByOneDimension(
                image_.Width(), hth_max);
        auto number_of_Y_components_v = GetNumberOfComponentsByOneDimension(
                image_.Height(), vth_max);

        std::vector<std::vector<int>> MCU;
        for (size_t i = 0; i < number_of_Y_components_h; ++i) {
            for (size_t j = 0; j < number_of_Y_components_v; ++j) {
                for (size_t i_1 = 0; i_1 < components_[0].vth; ++i_1) {
                    std::vector<std::vector<int>> row_of_blocks(8);
                    for (size_t j_1 = 0; j_1 < components_[0].hth; ++j_1) {
                        std::vector<std::vector<int>> comp(8, std::vector<int>(8));
                        ReadDC(comp, 0);
                        ReadAC(comp, 0);
                        Dequant(comp, components_[0].qt_id);
                        InplaceHstack(row_of_blocks, comp);
                    }
                    InplaceVstack(MCU, row_of_blocks);
                }
            }
        }

        int counter = 0;
        /*while (true) {
            __print__(counter++);
            file_.GetByte();
        }*/

    }

    void EOI() {
        AssertNextWord(0xFFD9, "Expected End of Image");
    }

    void GetCurrStructureLen() {
        curr_struct_len = static_cast<uint16_t>(file_.GetWord()) - 2;
    }

    void AssertBit(uint val) {
        if ((val & static_cast<uint>(-2)) != 0) {
            throw std::runtime_error("Expected 1 or 0");
        }
    }

    void AssertNextByte(uint8_t val, const std::string& error_message = std::string()) {
        auto byte = file_.GetByte();
        if (byte != val) {
            throw std::runtime_error(error_message);
        }
    }

    void AssertNextWord(uint16_t val, const std::string& error_message = std::string()) {
        auto word = file_.GetWord();
        if (word != val) {
            throw std::runtime_error(error_message);
        }
    }

    static void RunTests();

private:
    enum TableType {
        DC = 0,
        AC = 1
    };

    File file_;
    size_t curr_struct_len;

    Image image_;

    std::vector<std::vector<int>> Y_;
    std::vector<std::vector<int>> Cb_;
    std::vector<std::vector<int>> Cr_;

    std::vector<std::vector<std::vector<int>>> quantification_tables_;

    struct Component {
        Component(size_t id, size_t hth, size_t vth, size_t qt_id)
                : id(id), hth(hth), vth(vth), qt_id(qt_id) {}

        size_t id;
        size_t hth;
        size_t vth;
        size_t qt_id;

        int last_DC = 0;
        std::vector<HuffmanTree> trees;

        friend bool operator==(const Component& lhs, const Component& rhs) {
            return lhs.id == rhs.id && lhs.hth == rhs.hth
                   && lhs.vth == rhs.vth && lhs.qt_id == rhs.qt_id;
        }
    };

    // SOF0
    size_t precision_;
    size_t numer_of_components_;
    std::vector<Component> components_;

    size_t hth_max = 0;
    size_t vth_max = 0;

    std::vector<std::vector<std::vector<std::list<uint8_t>>>> tables_;
    std::vector<std::vector<HuffmanTree>> trees_;

    void FillQTWith(std::vector<std::vector<int>>& quantification_table,
                    File& file, std::function<uint8_t(File&)>&& func,
                    size_t size) {
        quantification_table.resize(size, std::vector<int>(size));
        for (size_t j = 0; j < size; ++j) {
            for (size_t i = 0; i <= j; ++i) {
                if (j % 2) {
                    quantification_table[i][j - i] = func(file);
                } else {
                    quantification_table[j-i][i] = func(file);
                }
            }
        }

        bool is_odd = size % 2;

        for (size_t j = 1; j < size; ++j) {
            for (size_t i = size - 1; i >= j; --i) {
                if ((j + is_odd) % 2) {
                    quantification_table[i][j + size - 1 - i] = func(file);
                } else {
                    quantification_table[j + size - 1 - i][i] = func(file);
                }
            }
        }
    }

    void ReadDC(std::vector<std::vector<int>>& block, size_t component_id) {
        size_t coef_size = components_[component_id].trees[DC].DecodeNext(&file_);
        if (coef_size == 0) {
            block[0][0] = 0;
        } else {
            int coef;
            if (coef_size != 255) {
                ASSERT(coef_size < sizeof(int) * 8, "Unexpected size of coefficient");
                coef = GetCoef(coef_size);
            } else {
                coef = 255;
            }
            coef += components_[component_id].last_DC;
            block[0][0] = coef;
            components_[component_id].last_DC = coef;
        }
    }

    void ReadAC(std::vector<std::vector<int>>& block, size_t component_id) {
        assert(block.size());
        size_t number_of_zeros = 0;
        size_t coef_size = 0;
        bool was_continued = false;
        int coef;
        for (size_t j = 0; j < block.size(); ++j) {
            for (size_t i = 0; i <= j; ++i) {
                if (!i && !j) {
                    continue;
                }

                if (!was_continued) {
                    int8_t byte = components_[component_id].trees[AC].DecodeNext(&file_);
                    if (!byte) {
                        return;
                    }
                    number_of_zeros = (byte & 0b11110000) >> 4;
                    coef_size = byte & 0b00001111;
                    coef = GetCoef(coef_size);
                }

                if (number_of_zeros > 0) {
                    --number_of_zeros;
                    was_continued = true;
                    continue;
                }

                was_continued = false;

                if (j % 2) {
                    block[i][j - i] = coef;
                } else {
                    block[j-i][i] = coef;
                }
            }
        }

        bool is_odd = block.size() % 2;

        for (size_t j = 1; j < block.size(); ++j) {
            for (size_t i = block.size() - 1; i >= j; --i) {
                if (!was_continued) {
                    int8_t byte = components_[component_id].trees[AC].DecodeNext(&file_);
                    if (!byte) {
                        return;
                    }
                    number_of_zeros = (byte & 0b11110000) >> 4;
                    coef_size = byte & 0b00001111;
                    coef = GetCoef(coef_size);
                }

                if (number_of_zeros > 0) {
                    --number_of_zeros;
                    was_continued = true;
                    continue;
                }

                was_continued = false;

                if ((j + is_odd) % 2) {
                    block[i][j + block.size() - 1 - i] = coef;
                } else {
                    block[j + block.size() - 1 - i][i] = coef;
                }
            }
        }
    }

    template <typename T>
    void Dequant(std::vector<std::vector<T>>& block, size_t qt_id) {
        for (size_t i = 0; i < block.size(); ++i) {
            for (size_t j = 0; j < block.size(); ++j) {
                block[i][j] *= quantification_tables_[qt_id][i][j];
            }
        }
    }

    int GetCoef(size_t size) {
        int coef = 0;
        for (size_t i = 0; i < size; ++i) {
            coef |= static_cast<int>(file_.GetBit()) << (size - i - 1);
        }
        if (!(coef & (1 << (size - 1)))) {
            coef -= (1 << size) - 1;
        }
        return coef;
    }

    size_t GetNumberOfComponentsByOneDimension(size_t size, size_t thinning) {
        size_t thinned_size = size / thinning + (size / thinning * thinning != size);
        size_t number_of_components = (thinned_size) / 8 + (thinned_size / 8 * 8 != thinned_size);

        return number_of_components;
    }

    template <typename T>
    void InplaceHstack(std::vector<std::vector<T>>& left,
                       const std::vector<std::vector<T>>& right) {
        ASSERT(left.size() == right.size(), "Incompatible blocks");
        for (size_t i = 0; i < left.size(); ++i) {
            left[i].insert(left[i].end(), right[i].begin(), right[i].end());
        }
    }

    template <typename T>
    void InplaceVstack(std::vector<std::vector<T>>& top,
                       const std::vector<std::vector<T>>& bot) {
        top.insert(top.end(), bot.begin(), bot.end());
    }
};
