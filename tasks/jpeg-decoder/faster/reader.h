#pragma once

#include <vector>
#include <../bit_reader.h>
#include <../fields.h>
#include <huffman.h>
#include <cstdint>
#include <algorithm>
#include <glog/logging.h>
#include <optional>

class JpegReader {
public:
    JpegReader(BitReader& bit_reader);

    void Read();

    void ReadComment(uint16_t size);

    void ReadAPPn(uint16_t size);

    void ReadQTable(uint16_t size);

    void ReadStats(uint16_t size);

    void ReadHuffman(uint16_t size);

    void ReadStartOfScan(uint16_t size);

    void CheckFields();

    void Scan(std::vector<std::vector<double>>& output);

    void CheckEnd();

    int8_t ChannelNum(int index);

    Stats& GetStats();

private:
    BitReader& bit_reader_;
    std::vector<std::optional<QuantTable>> qtables_;
    Stats stats_;
    std::vector<std::optional<HuffmanTree>> dc_;
    std::vector<std::optional<HuffmanTree>> ac_;
};
