#pragma once

#include <array>

template <int N>
struct Arr {
    constexpr Arr() : arr_() {
        for (int i = 0; i < N; i++) {
            arr_[i] = false;
        }
    }
    constexpr Arr(const Arr<N>& rhs, int pos) : arr_() {
        for (int i = 0; i < N; i++) {
            arr_[i] = rhs.arr_[i];
        }
        arr_[pos] = true;
    }

    bool arr_[N];
};

template <int N>
constexpr int forbidden_determinant(const std::array<std::array<int, N>, N>& a,
                                    const Arr<N>& forbidden_lines, const Arr<N>& forbidden_columns,
                                    int forbidden_cnt) {
    if (N - forbidden_cnt == 1) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (!forbidden_lines.arr_[i] && !forbidden_columns.arr_[j]) {
                    return a[i][j];
                }
            }
        }
    }
    for (int i = 0; i < N; i++) {
        if (!forbidden_lines.arr_[i]) {
            int det = 0;
            int cnt = 0;
            for (int j = 0; j < N; j++) {
                if (!forbidden_columns.arr_[j]) {
                    if (cnt % 2 == 0) {
                        det += a[i][j] * forbidden_determinant<N>(a, Arr<N>(forbidden_lines, i),
                                                                  Arr<N>(forbidden_columns, j),
                                                                  forbidden_cnt + 1);
                    } else {
                        det -= a[i][j] * forbidden_determinant<N>(a, Arr<N>(forbidden_lines, i),
                                                                  Arr<N>(forbidden_columns, j),
                                                                  forbidden_cnt + 1);
                    }
                    cnt++;
                }
            }
            return det;
        }
    }
    return 0;
}

template <int N>
constexpr int determinant(const std::array<std::array<int, N>, N>& a) {
    constexpr auto forbidden_lines = Arr<N>();
    constexpr auto forbidden_columns = Arr<N>();

    return forbidden_determinant<N>(a, forbidden_lines, forbidden_columns, 0);
}
