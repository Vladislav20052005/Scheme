#include "is_prime.h"
#include <cmath>
#include <algorithm>
#include <thread>
#include <atomic>
#include <vector>

std::atomic<bool> result;

void Find(uint64_t from, uint64_t to, uint64_t x) {
    for (auto i = from; i < to; ++i) {
        if (x % i == 0) {
            result = false;
            return;
        }
    }
}

bool IsPrime(uint64_t x) {
    result = true;
    if (x <= 1) {
        return false;
    }
    uint64_t root = sqrt(x);
    uint64_t bound = std::min(root + 6, x);
    size_t thread_cnt = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for (uint64_t from = 2ull; from < bound; from += (bound / thread_cnt)) {
        threads.push_back(std::thread(Find, from, std::min(bound, from + bound / thread_cnt), x));
    }
    for (size_t i = 0; i < threads.size(); i++) {
        threads[i].join();
    }
    return result;
}
