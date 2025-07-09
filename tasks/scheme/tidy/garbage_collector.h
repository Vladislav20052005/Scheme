#pragma once

#include "scheme.h"
#include "object.h"
#include <map>

class Collector {
public:
    template <class T, class... Args>
    T* Make(Args&&... args) {
        T* ptr = new T(std::forward<Args>(args)...);
        view_[ptr] = 0;
        return ptr;
    }

    void Mark(std::vector<std::shared_ptr<Variable>>& storage) {
        for (auto ptr : storage) {
            
        }
    }

    void Collect() {

    }
    
private:
    std::map<Object*, size_t> view_;
};

Collector& GetCollector();


