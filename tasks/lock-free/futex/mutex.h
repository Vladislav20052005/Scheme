#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>

// Atomically do the following:
//    if (*(uint64_t*)addr == expected_value) {
//        sleep_on_address(addr)
//    }
void FutexWait(void *addr, uint64_t expected_value);

// Wakeup 1 thread sleeping on the given address
void FutexWakeOne(void *addr);

// Wakeup all threads sleeping on the given address
void FutexWakeAll(void *addr);

class Mutex {
public:
    void Lock() {
        std::atomic<int> c;
        if ((c.compare_exchange_weak(ongoing_, 0, 1)) != 0) {
            if (c != 2) {
                c.exchange(ongoing_, 2);
            }
        }
    }

    void Unlock() {
        ongoing_ = 0;
        FutexWakeOne(&ongoing_);
    }

private:
    std::atomic<int> ongoing_ = 0;
};
