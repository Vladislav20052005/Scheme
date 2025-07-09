#include <../reader.h>

#define M_BEGIN 0xffd8
#define M_END 0xffd9
#define M_COMMENT 0xfffe
#define M_QTABLE 0xffdb
#define M_BASELINE 0xffc0
#define M_HUFFMAN 0xffc4
#define M_SOSCAN 0xffda

#define M_APPN 0xffe0
#define M_APPN_M 0xfff0

JpegReader::JpegReader(BitReader& bit_reader)
    : bit_reader_(bit_reader), qtables_(16), dc_(16), ac_(16) {
}

void JpegReader::Read() {
    bool was_in_sof = false;
    while (true) {
        uint16_t fl = bit_reader_.GetHalfWord();
        switch (fl) {
            case M_BEGIN:
                break;
            case M_END:
                throw std::invalid_argument("END marker was visited to early");
                break;
            case M_COMMENT:
                ReadComment(bit_reader_.GetHalfWord() - 2);
                break;
            case M_QTABLE:
                ReadQTable(bit_reader_.GetHalfWord() - 2);
                break;
            case M_BASELINE:
                if (was_in_sof) {
                    throw std::invalid_argument("double sof");
                }
                was_in_sof = true;
                ReadStats(bit_reader_.GetHalfWord() - 2);
                break;
            case M_HUFFMAN:
                ReadHuffman(bit_reader_.GetHalfWord() - 2);
                break;
            case M_SOSCAN:
                ReadStartOfScan(bit_reader_.GetHalfWord() - 2);
                if (!was_in_sof) {
                    throw std::invalid_argument("wasn't in sof");
                }
                return;
            default:
                if ((fl & M_APPN_M) == M_APPN) {
                    ReadAPPn(bit_reader_.GetHalfWord() - 2);

                } else {
                    throw std::invalid_argument("no flag");
                }
        }
    }
}

void JpegReader::ReadComment(uint16_t size) {
    stats_.comment_.resize(size);
    for (size_t i = 0; i < size; i++) {
        stats_.comment_[i] = bit_reader_.GetByte();
    }
}

void JpegReader::ReadAPPn(uint16_t size) {
    for (size_t i = 0; i < size; i++) {
        bit_reader_.GetByte();
    }
}

void JpegReader::ReadQTable(uint16_t size) {
    int sizei = 0;
    while (sizei < size) {
        int value_length = bit_reader_.GetHalfByte();
        int id = bit_reader_.GetHalfByte();
        if (value_length == 0) {
            qtables_[id].emplace();
            for (size_t i = 0; i < 64; i++) {
                qtables_[id]->data_.push_back(bit_reader_.GetByte());
            }
        } else {
            qtables_[id].emplace();
            for (size_t i = 0; i < 64; i++) {
                qtables_[id]->data_.push_back(bit_reader_.GetHalfWord());
            }
        }
        sizei += (1 + 64);
    }
    if (sizei != size) {
        throw std::invalid_argument("incorrect size");
    }
}

void JpegReader::ReadStats(uint16_t size) {
    stats_.precision_ = bit_reader_.GetByte();
    stats_.height_ = bit_reader_.GetHalfWord();
    stats_.width_ = bit_reader_.GetHalfWord();
    stats_.channel_cnt_ = bit_reader_.GetByte();
    size -= 6;

    if (1 > stats_.channel_cnt_ || stats_.channel_cnt_ > 3) {
        throw std::invalid_argument("incorrect channel cnt");
    }
    if (size / 3 != stats_.channel_cnt_) {
        throw std::invalid_argument("incorrect channel cnt");
    }

    stats_.mcu_block_cnt_ = 0;

    int16_t h_max = 1, v_max = 1;
    for (size_t i = 0; i < size / 3; i++) {
        int16_t id = bit_reader_.GetByte();
        int16_t h = bit_reader_.GetHalfByte();
        int16_t v = bit_reader_.GetHalfByte();
        stats_.channels_[id].emplace();
        stats_.channels_[id]->h_ = h;
        stats_.channels_[id]->v_ = v;
        if ((h != 1 && h != 2) || (v != 1 && v != 2)) {
            throw std::invalid_argument("h/v sampling factor is incorrect");
        }
        stats_.mcu_block_cnt_ += h * v;
        if (h_max < h) {
            h_max = h;
        }
        if (v_max < v) {
            v_max = v;
        }
        stats_.channels_[id]->qtable_id_ = bit_reader_.GetByte();
    }
    for (size_t i = 1; i < 4; i++) {
        if (stats_.channels_[i] == std::nullopt) {
            stats_.channels_[i]->h_sampling_ = 1;
            stats_.channels_[i]->v_sampling_ = 1;
            continue;
        }
        stats_.channels_[i]->h_sampling_ = h_max / stats_.channels_[i]->h_;
        stats_.channels_[i]->v_sampling_ = v_max / stats_.channels_[i]->v_;
    }
    stats_.h_max_ = h_max;
    stats_.v_max_ = v_max;
    stats_.mcu_width_ = h_max * 8;
    stats_.mcu_height_ = v_max * 8;
    stats_.img_width_in_mcu_ =
        stats_.width_ / stats_.mcu_width_ + ((stats_.width_ % stats_.mcu_width_ != 0) ? 1 : 0);
    stats_.img_height_in_mcu_ =
        stats_.height_ / stats_.mcu_height_ + ((stats_.height_ % stats_.mcu_height_ != 0) ? 1 : 0);
    stats_.img_mcu_cnt_ = stats_.img_width_in_mcu_ * stats_.img_height_in_mcu_;
    stats_.img_block_cnt_ = stats_.img_mcu_cnt_ * stats_.mcu_block_cnt_;
}

void JpegReader::ReadHuffman(uint16_t size) {
    int sizei = 0;
    while (sizei < size) {
        uint8_t kclass = bit_reader_.GetHalfByte();
        uint8_t id = bit_reader_.GetHalfByte();
        std::vector<uint8_t> cnt(16);
        size_t summ = 0;
        for (size_t i = 0; i < 16; i++) {
            cnt[i] = bit_reader_.GetByte();
            summ += cnt[i];
        }
        std::vector<uint8_t> values(summ);
        for (size_t i = 0; i < summ; i++) {
            values[i] = bit_reader_.GetByte();
        }
        if (kclass == 0) {
            dc_[id].emplace();
            dc_[id]->Build(cnt, values);
        } else {
            ac_[id].emplace();
            ac_[id]->Build(cnt, values);
        }
        sizei += (1 + 16 + values.size());
    }
    if (sizei != size) {
        throw std::invalid_argument("incorrect size");
    }
}

void JpegReader::ReadStartOfScan(uint16_t size) {
    size_t channel_cnt = bit_reader_.GetByte();
    size -= 1;
    for (size_t i = 0; i < channel_cnt; i++) {
        uint8_t id = bit_reader_.GetByte();
        int dc = bit_reader_.GetHalfByte();
        int ac = bit_reader_.GetHalfByte();
        stats_.channels_[id]->dc_id_ = dc;
        stats_.channels_[id]->ac_id_ = ac;
    }
    size -= 2 * channel_cnt;
    if (bit_reader_.GetByte() != 0 || bit_reader_.GetByte() != 63 || bit_reader_.GetByte() != 0) {
        throw std::invalid_argument("non-baseline coding");
    }
    size -= 3;
    if (size != 0) {
        throw std::invalid_argument("incorrect size");
    }
    CheckFields();
}

void JpegReader::CheckFields() {
    for (size_t i = 0; i < 4; i++) {
        if (stats_.channels_[i] != std::nullopt) {
            if (qtables_[stats_.channels_[i]->qtable_id_] == std::nullopt) {
                throw std::invalid_argument("no qtable");
            }
            if (dc_[stats_.channels_[i]->dc_id_] == std::nullopt) {
                throw std::invalid_argument("no dc huffman");
            }
            if (ac_[stats_.channels_[i]->ac_id_] == std::nullopt) {
                throw std::invalid_argument("no dc huffman");
            }
        }
    }
}

void JpegReader::Scan(std::vector<std::vector<double>>& output) {
    std::vector<int> last_dcs(4, 0);
    output.resize(stats_.img_block_cnt_);
    for (size_t j = 0; j < stats_.img_block_cnt_; j++) {
        int channel_num = ChannelNum(j);
        int qtable_id = stats_.channels_[channel_num]->qtable_id_;
        std::vector<double> block(64, 0);
        {
            int value;
            int huffman_id = stats_.channels_[channel_num]->dc_id_;
            while (!dc_[huffman_id]->Move(bit_reader_.GetBit(), value)) {
            }

            if (value != 0) {
                bool first_bit;
                int dc = bit_reader_.GetBits(value, first_bit);
                if (!first_bit) {
                    dc = dc - (1 << value) + 1;
                }
                block[0] = (dc + last_dcs[channel_num]) * qtables_[qtable_id]->data_[0];
                last_dcs[channel_num] += dc;
            } else {
                block[0] = (last_dcs[channel_num]) * qtables_[qtable_id]->data_[0];
            }
        }
        int huffman_id = stats_.channels_[channel_num]->ac_id_;
        size_t i = 1;
        while (i < 64) {
            int value;
            while (!ac_[huffman_id]->Move(bit_reader_.GetBit(), value)) {
            }
            if (value != 0) {
                uint8_t first_half, second_half;
                BitReader::Slice(value, first_half, second_half);

                i += first_half;
                size_t valuel = second_half;

                bool first_bit;
                int ac = bit_reader_.GetBits(valuel, first_bit);

                if (!first_bit) {
                    ac = ac - (1 << valuel) + 1;
                }
                if (i > 64) {
                    throw std::invalid_argument("block overflow");
                }
                if (i == 64) {
                    break;
                }
                block[i] = ac * qtables_[qtable_id]->data_[i];
            } else {
                break;
            }
            i++;
        }
        output[j] = std::move(block);
    }
}

void JpegReader::CheckEnd() {
    if (bit_reader_.GetHalfWord() != M_END) {
        throw std::invalid_argument("no end marker");
    }
}

int8_t JpegReader::ChannelNum(int index) {
    index %= stats_.mcu_block_cnt_;
    for (size_t ret = 1; ret < stats_.channels_.size(); ret++) {
        index -= stats_.channels_[ret]->h_ * stats_.channels_[ret]->v_;
        if (index < 0) {
            if (ret < 1 || ret > 3) {
                throw 12;
            }
            return ret;
        }
    }
    throw std::invalid_argument("");
}

Stats& JpegReader::GetStats() {
    return stats_;
}