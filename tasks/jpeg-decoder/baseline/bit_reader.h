#pragma once

#include <istream>
#include <glog/logging.h>
class BitReader {
public:
    BitReader(std::istream& stream);

    uint8_t GetBit();

    uint16_t GetBits(size_t cnt, bool& first_bit);

    uint8_t GetHalfByte();

    uint8_t GetByte();

    int GetHalfWord();

    static void Slice(uint8_t input, uint8_t& first_half, uint8_t& second_half);

private:
    std::istream& stream_;
    uint8_t buffer_;
    uint8_t last_byte_ = 0;
    size_t buffer_ptr_ = 0;
};