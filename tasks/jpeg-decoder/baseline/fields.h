#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <optional>

struct QuantTable {
    std::vector<float> data_;
};

struct Channel {
    int8_t id_;
    int8_t h_;
    int8_t v_;
    int8_t qtable_id_;
    int8_t dc_id_;
    int8_t ac_id_;
    int8_t h_sampling_;
    int8_t v_sampling_;
};

struct Stats {
    Stats();
    int8_t precision_;
    int16_t width_;
    int16_t height_;
    int16_t h_max_;
    int16_t v_max_;
    size_t mcu_width_;
    size_t mcu_height_;
    size_t img_width_in_mcu_;
    size_t img_height_in_mcu_;
    size_t img_mcu_cnt_;
    size_t mcu_block_cnt_;
    size_t img_block_cnt_;
    int8_t channel_cnt_;
    std::vector<std::optional<Channel>> channels_;
    std::string comment_;
};
