#pragma once

#include <memory>

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;
};

class Number : public Object {
public:
    Number(long long value) : value_(value) {
    }
    long long GetValue() const {
        return value_;
    }

private:
    long long value_;
};

class Symbol : public Object {
public:
    Symbol(std::string name) : name_(name) {
    }
    const std::string& GetName() const {
        return name_;
    }

private:
    std::string name_;
};

class Cell : public Object {
public:
    Cell() : first_(nullptr), second_(nullptr) {
    }
    Cell(std::shared_ptr<Object> first) : first_(first), second_(nullptr) {
    }
    Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second)
        : first_(first), second_(second) {
    }
    std::shared_ptr<Object> GetFirst() const {
        return first_;
    }
    std::shared_ptr<Object> GetSecond() const {
        return second_;
    }
    void SetFirst(std::shared_ptr<Object> first) {
        first_ = first;
    }
    void SetSecond(std::shared_ptr<Object> second) {
        second_ = second;
    }

private:
    std::shared_ptr<Object> first_;
    std::shared_ptr<Object> second_;
};

///////////////////////////////////////////////////////////////////////////////

// Runtime type checking and conversion.
// This can be helpful: https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    auto a = std::dynamic_pointer_cast<T>(obj);
    return a.get() != nullptr;
}
