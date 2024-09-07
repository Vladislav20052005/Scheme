#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>

// Atomically do the following:
//    if (*(uint64_t*)value == expected_value) {
//        sleep_on_address(value)
//    }
void FutexWait(void *value, uint64_t expected_value);

// Wakeup 1 thread sleeping on address of value
void FutexWakeOne(void *value);

// Wakeup all threads sleeping on address of value
void FutexWakeAll(void *value);

class Mutex {
public:
    void Lock() {
        mutex_.lock();
    }

    void Unlock() {
        mutex_.unlock();
    }

private:
    std::mutex mutex_;
};
