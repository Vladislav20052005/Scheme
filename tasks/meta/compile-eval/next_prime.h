#pragma once

constexpr bool is_prime(int x) {
    if (x == 1) {
        return false;
    }
    for (int i = 2; i < 35000; i++) {
        if (i != x && x % i == 0) {
            return false;
        }
    }
    return true;
}

constexpr int next_prime(int x) {
    for (int next_prime = x; true; next_prime++) {
        if (is_prime(next_prime)) {
            return next_prime;
        }
    }
}
