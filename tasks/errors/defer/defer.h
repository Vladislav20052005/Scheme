#pragma once

#include <utility>
#include <cstddef>
#include <functional>

template <typename Callback>
class Defer final {
public:
    Defer(){};

    Defer(Callback callback) {
        new (GetCallbackBuffer()) Callback(std::move(callback));
    };

    ~Defer() {
        if (!called_) {
            std::invoke(std::move(GetCallback()));
            called_ = true;
        }
    }

    void Cancel() {
        Callback cb2 = std::move(GetCallback());
        called_ = true;
    }

    void Invoke() {
        (*this).~Defer();
    }

    void* GetCallbackBuffer() {
        return static_cast<void*>(callback_buffer_);
    }
    Callback& GetCallback() {
        return *reinterpret_cast<Callback*>(GetCallbackBuffer());
    }

private:
    alignas(Callback) char callback_buffer_[sizeof(Callback)];
    bool called_ = false;
};

template <typename Callback>
Defer(Callback) -> Defer<Callback>;
