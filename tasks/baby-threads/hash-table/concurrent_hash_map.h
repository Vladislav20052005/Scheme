#pragma once

#include <unordered_map>
#include <mutex>
#include <vector>
#include <list>
#include <functional>
#include <atomic>
#include <iostream>

template <class K, class V, class Hash = std::hash<K>>
class ConcurrentHashMap {
public:
    ConcurrentHashMap(const Hash& hasher = Hash()) : ConcurrentHashMap(kUndefinedSize, hasher) {
    }

    explicit ConcurrentHashMap(int expected_size, const Hash& hasher = Hash())
        : ConcurrentHashMap(expected_size, kDefaultConcurrencyLevel, hasher) {
    }

    ConcurrentHashMap(int expected_size, int expected_threads_count, const Hash& hasher = Hash())
        : lock_(std::max(expected_size, expected_threads_count)), hasher_(hasher), size_(0) {
        table_.resize(lock_.size());
        bucket_cnt_.store(lock_.size());
    }

    bool Insert(const K& key, const V& value) {
        if (Find(key).first) {
            return false;
        }
        std::unique_lock<std::mutex> lock(lock_[hasher_(key) % lock_.size()]);

        size_t index = hasher_(key) % bucket_cnt_.load();
        table_[index].push_back(std::make_pair(key, value));
        size_++;
        lock.unlock();
        if (size_.load() > 3 * bucket_cnt_.load()) {
            Rehash();
        }
        return true;
    }

    bool Erase(const K& key) {
        std::lock_guard<std::mutex> lock(lock_[hasher_(key) % lock_.size()]);

        size_t index = hasher_(key) % bucket_cnt_.load();
        for (auto iter = table_[index].begin(); iter != table_[index].end(); ++iter) {
            auto element = *iter;
            if (element.first == key) {
                table_[index].erase(iter);
                size_--;
                return true;
            }
        }
        return false;
    }

    void Clear() {
        for (auto& mutex : lock_) {
            mutex.lock();
        }
        size_.store(0);
        for (auto& element : table_) {
            element.clear();
        }
        for (auto iter = lock_.rbegin(); iter != lock_.rend(); ++iter) {
            (*iter).unlock();
        }
    }

    std::pair<bool, V> Find(const K& key) const {
        std::lock_guard<std::mutex> lock(lock_[hasher_(key) % lock_.size()]);
        size_t index = hasher_(key) % bucket_cnt_.load();
        for (auto& element : table_[index]) {
            if (element.first == key) {
                return std::make_pair(true, element.second);
            }
        }
        return std::make_pair(false, V());
    }

    const V At(const K& key) const {
        auto found_pair = Find(key);
        if (found_pair.first) {
            return found_pair.second;
        }
        throw std::out_of_range("");
    }

    size_t Size() const {
        return size_.load();
    }

    static const int kDefaultConcurrencyLevel;
    static const int kUndefinedSize;

private:
    void Rehash() {
        for (auto& mutex : lock_) {
            mutex.lock();
        }
        auto new_bucket_cnt = bucket_cnt_.load() * 3;
        std::vector<std::list<std::pair<K, V>>> new_table(new_bucket_cnt);
        for (size_t index = 0; index < bucket_cnt_.load(); index++) {
            for (auto element : table_[index]) {
                new_table[hasher_(element.first) % new_bucket_cnt].push_back(element);
            }
        }
        bucket_cnt_.store(new_bucket_cnt);
        table_ = std::move(new_table);
        for (auto iter = lock_.rbegin(); iter != lock_.rend(); ++iter) {
            (*iter).unlock();
        }
    }

private:
    std::vector<std::list<std::pair<K, V>>> table_;
    mutable std::vector<std::mutex> lock_;
    Hash hasher_;
    std::atomic<size_t> size_;
    std::atomic<size_t> bucket_cnt_;
};

template <class K, class V, class Hash>
const int ConcurrentHashMap<K, V, Hash>::kDefaultConcurrencyLevel = 8;

template <class K, class V, class Hash>
const int ConcurrentHashMap<K, V, Hash>::kUndefinedSize = 8;
