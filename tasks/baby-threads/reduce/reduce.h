#pragma once

#include <thread>
#include <mutex>
#include <cmath>
#include <vector>

template <class RandomAccessIterator, class T, class Func>
T Reduce(RandomAccessIterator first, RandomAccessIterator last, const T& initial_value, Func func) {
    size_t length = last - first;
    if (length == 1) {
        return *first;
    }
    if (length == 0) {
        return initial_value;
    }
    size_t max_thread_cnt = std::thread::hardware_concurrency();
    size_t thread_cnt = sqrt(length);
    if (thread_cnt > max_thread_cnt) {
        thread_cnt = max_thread_cnt;
    }
    std::vector<T> results(thread_cnt, initial_value);
    std::vector<std::thread> threads;

    size_t step = length / thread_cnt;
    if (length % thread_cnt != 0) {
        step++;
    }
    size_t num = 0;
    std::mutex mutex;
    for (auto iter = first; num < thread_cnt; iter += step) {
        threads.emplace_back(
            [&mutex, &initial_value, &func, &results, num](RandomAccessIterator first,
                                                           RandomAccessIterator last) {
                auto cur_value(initial_value);
                while (first != last) {
                    cur_value = func(cur_value, *first++);
                }
                std::lock_guard<std::mutex> guard(mutex);
                results[num] = cur_value;
            },
            iter, (iter + step < last) ? (iter + step) : last);
        num++;
    }
    for (auto& thread : threads) {
        thread.join();
    }
    auto first_result = results.begin();
    auto last_result = results.end();

    auto cur_value(initial_value);
    while (first_result != last_result) {
        cur_value = func(cur_value, *first_result++);
    }
    return cur_value;
}
