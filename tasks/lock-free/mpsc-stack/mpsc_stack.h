#pragma once

#include <atomic>
#include <optional>
#include <stdexcept>
#include <utility>

template <class T>
class MPSCStack {
public:
    void Push(const T& value) {
        Node* expected = nullptr;
        Node* new_head = new Node(value, nullptr);
        while (!head_.compare_exchange_weak(expected, new_head)) {
            new_head->next_ = expected;
        }
    }

    std::optional<T> Pop() {
        Node* expected = head_.load();
        while (expected != nullptr && !head_.compare_exchange_weak(expected, expected->next_)) {
        }
        if (expected == nullptr) {
            return std::nullopt;
        }

        T value = expected->value_;
        expected->next_ = nullptr;
        delete expected;
        return value;
    }

    template <class TFn>
    void DequeueAll(const TFn& cb) {
        while (head_ != nullptr) {
            std::optional<T> value = Pop();
            if (value != std::nullopt) {
                cb(value.value());
            }
        }
    }

    ~MPSCStack() {
        if (head_ != nullptr) {
            delete head_;
        }
    }

private:
    struct Node {
        Node(T value, Node* next) : value_(value), next_(next) {
        }
        ~Node() {
            if (next_ != nullptr) {
                delete next_;
            }
        }
        T value_;
        Node* next_ = nullptr;
    };
    std::atomic<Node*> head_;
};
