#include <../transforms.h>

void Convert(std::vector<std::vector<double>>& vec) {
    const std::vector<int> convert_i = {0, 0, 1, 2, 1, 0, 0, 1, 2, 3, 4, 3, 2, 1, 0, 0,
                                        1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3,
                                        4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6,
                                        7, 7, 6, 5, 4, 3, 4, 5, 6, 7, 7, 6, 5, 6, 7, 7};
    const std::vector<int> convert_j = {0, 1, 0, 0, 1, 2, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5,
                                        4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4,
                                        3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3,
                                        2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 5, 6, 7, 7, 6, 7};

    for (size_t block_i = 0; block_i < vec.size(); block_i++) {
        std::vector<double> output(64, 0);
        for (size_t index = 0; index < 64; index++) {
            output[convert_i[index] * 8 + convert_j[index]] = vec[block_i][index];
        }
        vec[block_i] = std::move(output);
    }
}

void DCTTransform(std::vector<std::vector<double>>& vec) {
    std::vector<double> in(64, 0);
    std::vector<double> out(64, 0);
    DctCalculator dct(8, &in, &out);
    for (size_t index = 0; index < vec.size(); index++) {
        in = vec[index];
        dct.Inverse();
        vec[index] = out;
        for (size_t j = 0; j < 64; j++) {
            vec[index][j] = static_cast<int>(vec[index][j]);
            vec[index][j] += 128;

            if (vec[index][j] > 255) {
                vec[index][j] = 255;
            }
            if (vec[index][j] < 0) {
                vec[index][j] = 0;
            }
        }
    }
}

void ConstructImage(std::vector<std::vector<double>>& input, Image& image, Stats& stats) {
    for (size_t i = 0; i < image.Height(); i++) {
        for (size_t j = 0; j < image.Width(); j++) {
            size_t mcu_i = i / (stats.v_max_ * 8);
            size_t mcu_j = j / (stats.h_max_ * 8);
            size_t mcu_index = mcu_i * stats.img_width_in_mcu_ + mcu_j;
            size_t block_index = mcu_index * stats.mcu_block_cnt_;

            size_t mcu_inner_i = (i / 8) % stats.v_max_;
            size_t mcu_inner_j = (j / 8) % stats.h_max_;
            size_t block_inner_i_y = (i / stats.channels_[1]->v_sampling_) % 8;
            size_t block_inner_j_y = (j / stats.channels_[1]->h_sampling_) % 8;
            size_t block_inner_i_c = (i / stats.channels_[2]->v_sampling_) % 8;
            size_t block_inner_j_c = (j / stats.channels_[2]->h_sampling_) % 8;

            size_t offset_y = (mcu_inner_i)*stats.channels_[1]->h_ + (mcu_inner_j);
            size_t offset_cb = stats.channels_[1]->h_ * stats.channels_[1]->v_;
            size_t offset_cr = stats.channels_[2]->h_ * stats.channels_[2]->v_ +
                               stats.channels_[1]->h_ * stats.channels_[1]->v_;

            double y = 0, cb = 128, cr = 128;
            if (stats.channels_[1] != std::nullopt) {
                y = input.at(block_index + offset_y).at(block_inner_i_y * 8 + block_inner_j_y);
            }
            if (stats.channels_[2] != std::nullopt) {
                cb = input.at(block_index + offset_cb).at(block_inner_i_c * 8 + block_inner_j_c);
            }
            if (stats.channels_[3] != std::nullopt) {
                cr = input.at(block_index + offset_cr).at(block_inner_i_c * 8 + block_inner_j_c);
            }
            image.SetPixel(i, j, ToRGB(y, cb, cr));
        }
    }
}

RGB ToRGB(double y, double cb, double cr) {
    RGB ret;
    ret.r = static_cast<int>(y + 1.402 * (cr - 128));
    ret.g = static_cast<int>(y - 0.34414 * (cb - 128) - 0.71414 * (cr - 128));
    ret.b = static_cast<int>(y + 1.772 * (cb - 128));
    if (ret.r < 0) {
        ret.r = 0;
    }
    if (ret.r > 255) {
        ret.r = 255;
    }
    if (ret.g < 0) {
        ret.g = 0;
    }
    if (ret.g > 255) {
        ret.g = 255;
    }
    if (ret.b < 0) {
        ret.b = 0;
    }
    if (ret.b > 255) {
        ret.b = 255;
    }
    return ret;
}