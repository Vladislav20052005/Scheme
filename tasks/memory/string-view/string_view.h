#pragma once

#include <string>
#include <cstring>
#include <algorithm>

class StringView {
public:
    StringView(const std::string& str) : ptr_(&str[0]), size_(str.size()) {
    }
    StringView(const std::string& str, size_t start)
        : ptr_(&str[start]), size_(str.size() - start) {
    }
    StringView(const std::string& str, size_t start, size_t size)
        : ptr_(&str[start]), size_(std::min(str.size() - start, size)) {
    }
    StringView(const char* str) : ptr_(str), size_(strlen(str)) {
    }
    StringView(const char* str, size_t size) : ptr_(str), size_(size) {
    }

    char operator[](size_t i) const {
        return *(ptr_ + i);
    }
    size_t Size() const {
        return size_;
    }

private:
    const char* ptr_;
    const size_t size_;
};
