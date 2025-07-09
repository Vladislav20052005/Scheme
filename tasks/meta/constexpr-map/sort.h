#pragma once

#include <constexpr_map.h>

#include <type_traits>

template <class K, class V, int S>
constexpr auto Sort(ConstexprMap<K, V, S> map) {
    ConstexprMap<K, V, S> ret_map = map;
    if constexpr (std::is_integral<K>::value) {
        for (size_t i = 0; i < ret_map.Size(); i++) {
            for (size_t j = 0; j < ret_map.Size() - i - 1; j++) {
                if (ret_map.GetByIndex(j + 1).first > ret_map.GetByIndex(j).first) {
                    ret_map.GetByIndex(j).swap(ret_map.GetByIndex(j + 1));
                }
            }
        }
    } else {
        for (size_t i = 0; i < ret_map.Size(); i++) {
            for (size_t j = 0; j < ret_map.Size() - i - 1; j++) {
                if (ret_map.GetByIndex(j + 1).first < ret_map.GetByIndex(j).first) {
                    ret_map.GetByIndex(j).swap(ret_map.GetByIndex(j + 1));
                }
            }
        }
    }
    return ret_map;
}
