#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

class DefaultCallback {
public:
    void operator()(int& value) {
        --value;
    }
};

class Semaphore {
public:
    Semaphore(int count) : count_(count) {
    }

    void Leave() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        cv_.notify_one();
    }

    template <class Func>
    void Enter(Func callback) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!count_ || !waiting_threads_.empty()) {
            int my_signature = GetSignature();
            waiting_threads_.push(my_signature);
            while (true) {
                cv_.wait(lock);
                if (waiting_threads_.front() == my_signature) {
                    waiting_threads_.pop();
                    callback(count_);
                    return;
                } else {
                    cv_.notify_one();
                }
            }
        }
        callback(count_);
    }

    void Enter() {
        DefaultCallback callback;
        Enter(callback);
    }

private:
    int GetSignature() {
        ++signature_gen_;
        return signature_gen_;
    }
    int signature_gen_ = 0;

private:
    std::queue<int> waiting_threads_;
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_ = 0;
};
