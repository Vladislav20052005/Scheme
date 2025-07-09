#include <../bit_reader.h>

BitReader::BitReader(std::istream& stream) : stream_(stream), buffer_(0), buffer_ptr_(0) {
}

uint8_t BitReader::GetBit() {
    if (buffer_ptr_ == 0) {
        last_byte_ = buffer_;
        buffer_ = stream_.get();
        if (last_byte_ == 255 && buffer_ == 0) {
            buffer_ = stream_.get();
        }
        if (stream_.eof()) {
            throw std::invalid_argument("no more bits");
        }
    }
    int ret = buffer_ & (1 << (7 - buffer_ptr_));
    buffer_ptr_++;
    buffer_ptr_ %= 8;
    if (ret) {
        return 1;
    }
    return 0;
}

uint16_t BitReader::GetBits(size_t cnt, bool& first_bit) {
    if (cnt > 16) {
        throw std::invalid_argument("wrong cnt");
    }
    first_bit = true;
    if (cnt == 0) {
        first_bit = true;
        return 0;
    }
    int16_t result = 0;
    result |= GetBit();
    first_bit = result;
    for (size_t i = 0; i < cnt - 1; i++) {
        result <<= 1;
        result |= GetBit();
    }
    return result;
}

uint8_t BitReader::GetHalfByte() {
    int ret;
    if (buffer_ptr_ == 0) {
        buffer_ = stream_.get();
        ret = (buffer_ & 0xf0) >> 4;
    } else {
        ret = buffer_ & 0x0f;
    }
    buffer_ptr_ += 4;
    buffer_ptr_ %= 8;
    return ret;
}

uint8_t BitReader::GetByte() {
    return stream_.get();
}

int BitReader::GetHalfWord() {
    return (stream_.get() << 8) | stream_.get();
}

void BitReader::Slice(uint8_t input, uint8_t& first_half, uint8_t& second_half) {
    first_half = (input & 0xf0) >> 4;
    second_half = input & 0x0f;
}
