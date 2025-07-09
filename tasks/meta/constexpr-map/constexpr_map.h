#pragma once

#include <cstddef>
#include <utility>

template <class K, class V, int MaxSize = 8>
class ConstexprMap {
public:
    constexpr ConstexprMap() = default;

    constexpr V& operator[](const K& key) {
        for (auto& pr : data_) {
            if (pr.first == key) {
                return pr.second;
            }
        }
        data_[next_free_] = std::pair<K, V>(std::make_pair(key, V()));
        next_free_++;
        return data_[next_free_ - 1].second;
    }

    constexpr const V& operator[](const K& key) const {
        for (auto& pr : data_) {
            if (pr.first == key) {
                return pr.second;
            }
        }
    }

    constexpr bool Erase(const K& key) {
        for (size_t i = 0; i < next_free_; i++) {
            auto pr = data_[i];
            if (pr.first == key) {
                for (size_t j = i + 1; j < next_free_; j++) {
                    data_[j - 1] = data_[j];
                }
                next_free_--;
                return true;
            }
        }
        return false;
    }

    constexpr bool Find(const K& key) const {
        for (auto& pr : data_) {
            if (pr.first == key) {
                return true;
            }
        }
        return false;
    }

    constexpr size_t Size() const {
        return next_free_;
    }

    constexpr std::pair<K, V>& GetByIndex(size_t pos) {
        return data_[pos];
    }

    constexpr const std::pair<K, V>& GetByIndex(size_t pos) const {
        return data_[pos];
    }

private:
    std::pair<K, V> data_[MaxSize];
    size_t next_free_ = 0;
};
