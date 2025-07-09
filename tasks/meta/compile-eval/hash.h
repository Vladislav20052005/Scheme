#pragma once

constexpr long long mod_pow(long long a, long long b, long long mod) {
    return (b == 0) ? 1 : (mod_pow(a, b - 1, mod) * a) % mod;
}

constexpr long long hashi(const char *s, long long p, long long mod, int i) {
    return (s[i] == '\0') ? 0 : (hashi(s, p, mod, i + 1) + s[i] * (mod_pow(p, i, mod) % mod)) % mod;
}

constexpr long long hash(const char *s, long long p, long long mod) {
    return hashi(s, p, mod, 0);
}
