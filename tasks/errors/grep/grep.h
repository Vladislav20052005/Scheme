#pragma once

#include <optional>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "utf8.h"

using std::optional;

struct GrepOptions {
    optional<size_t> look_ahead_length;
    size_t max_matches_per_line;

    GrepOptions() {
        max_matches_per_line = 10;
    }

    GrepOptions(size_t look_ahead_length) : GrepOptions() {
        this->look_ahead_length = look_ahead_length;
    }

    GrepOptions(optional<size_t> look_ahead_length, size_t max_matches_per_line) {
        this->look_ahead_length = look_ahead_length;
        this->max_matches_per_line = max_matches_per_line;
    }
};

template <class Visitor>
void Grep(const std::string& path, const std::string& pattern, Visitor visitor,
          const GrepOptions& options) {
    try {

        if (std::filesystem::is_directory(path) && std::filesystem::exists(path)) {
            for (const auto& file : std::filesystem::directory_iterator(path)) {
                Grep(file.path(), pattern, visitor, options);
            }
        } else if (!std::filesystem::is_block_file(path) &&
                   !std::filesystem::is_character_file(path) && !std::filesystem::is_fifo(path) &&
                   !std::filesystem::is_socket(path) && !std::filesystem::is_symlink(path)) {
            std::ifstream fin(path);

            std::string line;
            size_t line_num = 0;
            while (getline(fin, line)) {
                if (!utf8::is_valid(line.begin(), line.end())) {
                    return;
                }
                auto iter_start = line.begin();
                size_t found_in_this_line = 0;
                while (true) {
                    const auto it =
                        std::search(iter_start, line.end(),
                                    std::boyer_moore_searcher(pattern.begin(), pattern.end()));
                    if (it != line.end() && found_in_this_line < options.max_matches_per_line) {
                        if (options.look_ahead_length == std::nullopt) {
                            visitor.OnMatch(path, line_num + 1,
                                            utf8::distance(line.begin(), it) + 1, std::nullopt);
                        } else {
                            auto iter2 = it + pattern.size();
                            std::string context = "";
                            while (utf8::distance(it + pattern.size(), iter2) <
                                       options.look_ahead_length.value() &&
                                   iter2 < line.end()) {

                                utf8::next(iter2, line.end());
                            }

                            for (auto iter = it + pattern.size(); iter < iter2; ++iter) {
                                context += *iter;
                            }
                            visitor.OnMatch(path, line_num + 1,
                                            utf8::distance(line.begin(), it) + 1, context);
                        }
                        iter_start = it;
                        utf8::next(iter_start, line.end());
                        found_in_this_line++;
                    } else {
                        break;
                    }
                }
                line_num++;
            }
        }
    } catch (const std::exception& e) {
        visitor.OnError(e.what());
    }
    return;
}
