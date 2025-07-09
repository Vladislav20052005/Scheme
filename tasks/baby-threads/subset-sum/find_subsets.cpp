#include "find_subsets.h"
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <list>
#include <functional>
#include <atomic>
#include <algorithm>

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

uint64_t Power(uint64_t x, uint64_t p) {
    if (p == 0) {
        return 1;
    }
    uint64_t y = Power(x, p / 2);
    if (p % 2 == 0) {
        return y * y;
    }
    return y * y * x;
}

bool IsEmpty(size_t set1, size_t set2, size_t size1, size_t size2) {
    bool has_zeroes = false;
    bool has_twoes = false;
    for (size_t index = 0; index < size1; ++index) {
        has_zeroes = has_zeroes || (set1 % 3 == 0);
        has_twoes = has_twoes || (set1 % 3 == 2);
        set1 /= 3;
    }
    for (size_t index = 0; index < size2; ++index) {
        has_zeroes = has_zeroes || (set2 % 3 == 0);
        has_twoes = has_twoes || (set2 % 3 == 2);
        set2 /= 3;
    }
    return (!has_zeroes || !has_twoes);
}

Subsets FindEqualSumSubsets(const std::vector<int64_t>& data) {
    Subsets subsets;
    subsets.exists = false;
    if (data.empty()) {
        return subsets;
    }

    size_t first_half_size = data.size() / 2;
    size_t second_half_size = data.size() - first_half_size;

    size_t first_subset_cnt = Power(3, first_half_size);
    size_t second_subset_cnt = Power(3, second_half_size);

    size_t threads_count = 
        std::min(static_cast<size_t>(std::thread::hardware_concurrency()), first_subset_cnt);
    std::vector<std::thread> threads;

    std::unique_ptr<ConcurrentHashMap<int64_t, size_t>> table =
        std::make_unique<ConcurrentHashMap<int64_t, size_t>>(threads_count, first_subset_cnt);
    size_t concurrent_step = (first_subset_cnt / threads_count) + 1;
    for (size_t thread_index = 0; thread_index < threads_count; ++thread_index) {

        threads.push_back(std::thread(
            [&](size_t begin, size_t end) {
                for (size_t set = begin; set < end; ++set) {
                    size_t e = set;
                    int64_t sum = 0;
                    for (size_t index = 0; index < first_half_size; ++index) {
                        sum += data[index] * ((e % 3) - 1);
                        e /= 3;
                    }
                    //table->Insert(sum, set);
                }
            },
            thread_index * concurrent_step,
            std::min(first_subset_cnt, (thread_index + 1) * concurrent_step)));
    }
    for (auto& thread : threads) {
        thread.join();
    }
    threads.clear();

    size_t set1;
    size_t set2;
    bool exists = false;
    std::mutex m;
/*
    concurrent_step = (second_subset_cnt / threads_count) + 1;
    for (size_t thread_index = 0; thread_index < threads_count; ++thread_index) {
        threads.push_back(std::thread(
            [&](size_t begin, size_t end) {
                for (size_t set = begin; set < end; ++set) {
                    size_t e = set;
                    int64_t sum = 0;
                    for (size_t index = 0; index < second_half_size; ++index) {
                        sum += data[first_half_size + index] * ((e % 3) - 1);
                        e /= 3;
                    }
                    auto found = table->Find(-sum);
                    if (found.first) {
                        if (!IsEmpty(found.second, set, first_half_size, second_half_size)) {
                            std::lock_guard<std::mutex> lock(m);
                            set1 = found.second;
                            set2 = set;
                            exists = true;
                            return;
                        }
                    }
                }
            },
            thread_index * concurrent_step,
            std::min(second_subset_cnt, (thread_index + 1) * concurrent_step)));
    }

    for (auto& thread : threads) {
        thread.join();
    }
*/
    if (!exists) {
        return subsets;
    }
    subsets.exists = true;
    size_t e = set1;
    for (size_t index = 0; index < first_half_size; ++index) {
        if (e % 3 == 0) {
            subsets.first_indices.push_back(index);
        }
        if (e % 3 == 2) {
            subsets.second_indices.push_back(index);
        }
        e /= 3;
    }
    e = set2;
    for (size_t index = 0; index < second_half_size; ++index) {
        if (e % 3 == 0) {
            subsets.first_indices.push_back(first_half_size + index);
        }
        if (e % 3 == 2) {
            subsets.second_indices.push_back(first_half_size + index);
        }
        e /= 3;
    }
    return subsets;
}
