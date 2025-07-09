#pragma once

#include <utility>
#include <optional>
#include <mutex>
#include <atomic>
#include <condition_variable>

template <class T>
class UnbufferedChannel {
public:
    void Send(const T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (closed_) {
            throw std::runtime_error("channel is closed");
        }
        while (x_ != std::nullopt || ongoing_) {
            waiting_send_.wait(lock);
            if (closed_) {
                waiting_send_.notify_one();
                throw std::runtime_error("channel is closed");
            }
        }
        ongoing_ = true;
        x_ = value;
        waiting_recv_.notify_one();
        while (x_ != std::nullopt) {
            sender_recv_.wait(lock);
            if (closed_) {
                break;
            }
        }
        waiting_send_.notify_one();
        ongoing_ = false;
    }

    std::optional<T> Recv() {
        std::unique_lock<std::mutex> lock(mutex_);
        if (closed_ && x_ == std::nullopt) {
            return std::nullopt;
        }
        if (!ongoing_) {
            waiting_send_.notify_one();
        }
        while (x_ == std::nullopt) {
            waiting_recv_.wait(lock);
            if (closed_ && x_ == std::nullopt) {
                waiting_recv_.notify_one();
                return std::nullopt;
            }
        }
        T value = x_.value();
        x_ = std::nullopt;
        sender_recv_.notify_one();
        return value;
    }

    void Close() {
        std::unique_lock<std::mutex> lock(mutex_);
        closed_ = true;
        waiting_send_.notify_one();
        waiting_recv_.notify_one();
        sender_recv_.notify_one();
    }

private:
    std::mutex mutex_;
    std::optional<T> x_ = std::nullopt;
    std::atomic<bool> ongoing_ = false;
    std::atomic<bool> closed_ = false;
    std::condition_variable waiting_send_;
    std::condition_variable waiting_recv_;
    std::condition_variable sender_recv_;
};
