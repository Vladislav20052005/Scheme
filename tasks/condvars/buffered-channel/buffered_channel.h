#pragma once

#include <utility>
#include <optional>
#include <vector>
#include <mutex>
#include <condition_variable>

template <class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size) : data_(size) {
    }

    void Send(const T& value) {
        if (closed_) {
            throw std::runtime_error("channel is closed");
        }
        waiting_send_cnt_++;
        std::unique_lock<std::mutex> lock(mutex_);
        while (size_ == data_.size()) {
            waiting_send_.wait(lock);
        }
        waiting_send_cnt_--;
        Push(value);
        waiting_recv_.notify_all();
    }

    std::optional<T> Recv() {
        std::unique_lock<std::mutex> lock(mutex_);
        if (closed_ && size_ == 0 && waiting_send_cnt_ == 0) {
            return std::nullopt;
        }
        if (closed_ && (waiting_send_cnt_ + size_) == waiting_recv_cnt_) {
            return std::nullopt;
        }
        waiting_recv_cnt_++;
        while (size_ == 0) {
            waiting_recv_.wait(lock);
            if (closed_ && size_ == 0) {
                if (waiting_send_cnt_ == 0) {
                    return std::nullopt;
                }
                waiting_send_.notify_one();
            }
        }
        waiting_recv_cnt_--;
        T value = Pop();
        waiting_send_.notify_one();
        return value;
    }

    void Close() {
        std::unique_lock<std::mutex> lock(mutex_);
        closed_ = true;
        waiting_recv_.notify_all();
        waiting_send_.notify_all();
    }

private:
    void Push(const T& value) {
        data_[end_] = value;
        ++end_;
        ++size_;
        if (end_ == data_.size()) {
            end_ = 0;
        }
    }
    T Pop() {
        ++begin_;
        --size_;
        T value = std::move(data_[begin_ - 1]);
        if (begin_ == data_.size()) {
            begin_ = 0;
        }
        return value;
    }

private:
    std::atomic<bool> closed_ = false;
    size_t begin_ = 0;
    size_t end_ = 0;
    std::atomic<size_t> size_ = 0;
    std::vector<T> data_;
    std::atomic<size_t> waiting_send_cnt_ = 0;
    std::atomic<size_t> waiting_recv_cnt_ = 0;
    std::condition_variable waiting_send_;
    std::condition_variable waiting_recv_;
    std::mutex mutex_;
};
