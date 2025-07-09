#pragma once

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <map>
#include <queue>

using namespace std::chrono_literals;

template <class T>
class TimerQueue {
public:
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;
    struct Compare {
        bool operator()(std::pair<T, TimePoint> a, std::pair<T, TimePoint> b) {
            return a.second > b.second;
        }
    };

public:
    void Add(const T& item, TimePoint at) {
        std::unique_lock<std::mutex> lock(m_);
        queue_.push(std::make_pair(item, at));
        cv_.notify_all();
    }

    T Pop() {
        std::unique_lock<std::mutex> lock(m_);
        std::pair<T, TimePoint> top = GetTop();
        while (Clock::now() < top.second) {
            cv_.wait_until(lock, top.second);
            top = GetTop();
        }
        queue_.pop();
        return top.first;
    }

private:
    inline std::pair<T, TimePoint> GetTop() {
        if (queue_.empty()) {
            return std::make_pair(T(), Clock::now() + 1s);
        }
        return queue_.top();
    }

private:
    std::mutex m_;
    std::condition_variable cv_;
    std::priority_queue<std::pair<T, TimePoint>, std::vector<std::pair<T, TimePoint>>, Compare>
        queue_;
};
