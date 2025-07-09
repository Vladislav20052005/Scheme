#pragma once

#include <atomic>
#define LAST_BIT 0x8000000000000000

struct RWSpinLock {
    void LockRead() {
        unsigned long expected = val_.load() & (~LAST_BIT);
        while (!val_.compare_exchange_strong(expected, (expected + 1))) {
            expected = val_.load() & (~LAST_BIT);
        }
    }

    void UnlockRead() {
        val_.fetch_sub(1);
    }

    void LockWrite() {
        unsigned long expected = 0;
        while (!val_.compare_exchange_strong(expected, LAST_BIT)) {
            expected = 0;
        }
    }

    void UnlockWrite() {
        val_.store(0);
    }

    std::atomic<unsigned long> val_ = 0;
};
