#pragma once
#include <mutex>
#include <condition_variable>

class RWLock {
public:
    template <class Func>
    void Read(Func func) {
        read_.lock();
        ++blocked_readers_;
        read_.unlock();
        try {
            func();
        } catch (...) {
            EndRead();
            throw;
        }
        EndRead();
    }

    template <class Func>
    void Write(Func func) {
        std::unique_lock<std::mutex> lock(read_);
        while (blocked_readers_) {
            cv_.wait(lock);
        }
        func();
        cv_.notify_one();
    }

private:
    std::mutex read_;
    std::mutex global_;
    std::condition_variable cv_;
    int blocked_readers_ = 0;

    void EndRead() {
        read_.lock();
        --blocked_readers_;
        if (!blocked_readers_) {
            cv_.notify_one();
        }
        read_.unlock();
    }
};
