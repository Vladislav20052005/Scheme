#pragma once

#include <utility>
#include <functional>

template <typename Signature>
class FunctionRef;

template <typename ReturnType, typename... Args>
class FunctionRef<ReturnType(Args...)> {
public:
    FunctionRef() = delete;
    template <typename Func>
    FunctionRef(const Func& func)
        : pfunc_(const_cast<void*>(reinterpret_cast<const void*>(&func))) {
        trampoline_ = [](void* ptr, Args... args) -> ReturnType {
            return (*reinterpret_cast<Func*>(ptr))(std::forward<Args>(args)...);
        };
    }
    template <typename Func>
    FunctionRef(Func* func) : pfunc_(reinterpret_cast<void*>(func)) {
        trampoline_ = [](void* ptr, Args... args) -> ReturnType {
            return (*reinterpret_cast<Func*>(ptr))(std::forward<Args>(args)...);
        };
    }

    ReturnType operator()(Args... args) {
        return trampoline_(pfunc_, std::forward<Args>(args)...);
    }

private:
    void* pfunc_;
    ReturnType (*trampoline_)(void*, Args...);
};

template <typename... Args>
class FunctionRef<void(Args...)> {
public:
    FunctionRef() = delete;
    template <typename Func>
    FunctionRef(const Func& func)
        : pfunc_(const_cast<void*>(reinterpret_cast<const void*>(&func))) {
        trampoline_ = [](void* ptr, Args... args) -> void {
            (*reinterpret_cast<Func*>(ptr))(std::forward<Args>(args)...);
        };
    }
    template <typename Func>
    FunctionRef(Func* func) : pfunc_(reinterpret_cast<void*>(func)) {
        trampoline_ = [](void* ptr, Args... args) -> void {
            (*reinterpret_cast<Func*>(ptr))(std::forward<Args>(args)...);
        };
    }

    void operator()(Args... args) {
        return trampoline_(pfunc_, std::forward<Args>(args)...);
    }

private:
    void* pfunc_;
    void (*trampoline_)(void*, Args...);
};
