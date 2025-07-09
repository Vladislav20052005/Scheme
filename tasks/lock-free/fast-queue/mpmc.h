#pragma once

#include <atomic>
#include <vector>

template <class T>
class MPMCBoundedQueue {
public:
    explicit MPMCBoundedQueue(int size) : data_(size), gen_(size) {
        max_size_ = size;
        for (int i = 0; i < size; i++) {
            gen_[i] = i;
        }
    }

    bool Enqueue(const T& value) {
        int expected = tail_;
        if (gen_[tail_] == tail_) {
            while (gen_[tail_].compare_exchange_weak(expected, gen_[tail_] + 1)) {
                expected = tail_;
            }
        } else {
            return false;
        }
        data_[expected] = value;
        tail_++;
        return true;
    }

    bool Dequeue(T& data) {
        int expected = head_ + 1;
        if (gen_[head_] == head_ + 1) {
            while (gen_[head_].compare_exchange_weak(expected, head_ + max_size_)) {
                expected = head_ + 1;
            }
        } else {
            return false;
        }
        data = data_[head_];
        head_++;
        return true;
    }

private:
    std::vector<T> data_;
    std::vector<std::atomic<int>> gen_;
    size_t head_ = 0;
    size_t tail_ = 0;
    int max_size_ = 0;
};
