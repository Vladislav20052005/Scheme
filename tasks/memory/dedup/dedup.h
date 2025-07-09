#pragma once

#include <memory>
#include <vector>
#include <string>

using std::string;

std::vector<std::unique_ptr<string>> Duplicate(const std::vector<std::shared_ptr<string>>& items) {
    std::vector<std::unique_ptr<string>> ret;
    for (auto& ptr : items) {
        ret.push_back(std::make_unique<string>(*ptr));
    }
    return ret;
}

std::vector<std::shared_ptr<string>> DeDuplicate(
    const std::vector<std::unique_ptr<string>>& items) {
    std::vector<std::shared_ptr<string>> ret;
    for (auto& ptr : items) {
        bool first = true;
        for (auto& lptr : ret) {
            if (*ptr == *lptr) {
                first = false;
                ret.push_back(std::shared_ptr<string>(lptr));
                break;
            }
        }
        if (first) {
            ret.push_back(std::make_shared<string>(*ptr));
        }
    }

    return ret;
}
