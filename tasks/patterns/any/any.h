#pragma once

#include <memory>

class Any {
public:
    Any() : ptr_(nullptr) {
    }

    template <class T>
    Any(const T& value) : ptr_(new Base<T>(value)) {
    }

    template <class T>
    Any& operator=(const T& value) {
        ptr_.reset(new Base<T>(value));
        return *this;
    }

    Any(const Any& rhs) : ptr_((*rhs.ptr_).Copy()) {
    }

    Any& operator=(const Any& rhs) {
        ptr_.reset((*rhs.ptr_).Copy());
        return *this;
    }

    ~Any() = default;

    bool Empty() const {
        return ptr_.get() == nullptr;
    }

    void Clear() {
        ptr_.reset();
    }

    void Swap(Any& rhs) {
        rhs.ptr_.swap(ptr_);
    }

    template <class T>
    const T& GetValue() const {
        try {
            return *dynamic_cast<Base<T>&>(*ptr_);
        } catch (...) {
            throw;
        }
    }

    struct InnerBase {
        virtual ~InnerBase() = default;
        virtual InnerBase* Copy() = 0;
    };

    template <typename T>
    struct Base : InnerBase {
        Base(const T& value) : value_(value){};

        const T& operator*() const {
            return value_;
        }

        virtual InnerBase* Copy() override {
            return new Base<T>(value_);
        }

        T value_;
    };

private:
    std::unique_ptr<InnerBase> ptr_;
};
